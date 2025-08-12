# ***libasm***

## İçindekiler

- [Genel Bakış](#genel-bakış)
- [64-bit (x86-64) Register'lar ve Görevleri](#64-bit-x86-64-registerlar-ve-görevleri)
- [Korumalı ve Korumasız Register'lar](#korumalı-ve-korumasız-registerlar)
- [64-bit (x86-64) Assembly Temel Komutları (Intel Syntax)](#64-bit-x86-64-assembly-temel-komutları-intel-syntax)
- [Flags Register - İşlemci'nin Hafızası](#flags-register---i̇şlemcinin-hafızası)
- [Stack - PUSH/POP İşlemleri](#stack---pushpop-i̇şlemleri)
- [Section Türleri](#section-türleri)
- [Assembly Label'ları (Etiketler)](#assembly-labelları-etiketler)
- [PIE (Position Independent Executable) Nedir?](#pie-position-independent-executable-nedir)
- [Derleme ve Çalıştırma](#derleme-ve-çalıştırma)
- [Fonksiyonlar](#fonksiyonlar)

## Genel Bakış
>  **libasm**, dize manipülasyonu için basit ve hızlı bir assembly kütüphanesidir.
>
>  Tüm işlemler **register**'lar üzerinde gerçekleşir; fonksiyon argümanları ve dönüş değerleri doğrudan register'lar ile yönetilir.
>
>  Doğru register seçimi, kodun optimizasyonu için kritik öneme sahiptir.
>
>  Register'lar, CPU'nun ultra hızlı hafıza birimleridir; **RAM'den 100-200 kat daha hızlı** fakat sayıca sınırlıdır.
>
>  Geçici veri saklama, hesaplama ve işlem yönetimi için kullanılırlar.

## 64-bit (x86-64) Register'lar ve  Görevleri
| Register | Görev                                                      |
|----------|------------------------------------------------------------|
|  `rax`   | Ana işlemler için kullanılır (aritmetik, sonuçlar).        |
|  `rbx`   | Veri saklamak için genel amaçlı kullanılır.                |
|  `rcx`   | Sayaç olarak kullanılır (döngülerde).                      |
|  `rdx`   | Geniş veri işlemlerinde ve bazı fonksiyonlarda kullanılır. |
|  `rsi`   | Kaynak adresi (veri okuma işlemlerinde).                   |
|  `rdi`   | Hedef adresi (veri yazma işlemlerinde).                    |
|  `rbp`   | Fonksiyonlarda taban adresi (stack tabanı).                |
|  `rsp`   | Stack’in en üst adresi (stack pointer).                    |

### Register Alt-Parçaları (Sub-registers)

> Niçin kullanılır? 
> 
> Alt-parçalar, daha küçük veri boyutları ile işlem yapma imkanı tanır. Örneğin, bir byte'lık bir karakteri işlemek için 64-bit bir register kullanmak yerine, sadece 8-bit'lik bir alt-parça kullanmak daha verimlidir.

**Tüm genel amaçlı register'lar** aynı alt-parça sistemine sahiptir:

| Register          | 64-bit | 32-bit | 16-bit | 8-bit Alt | 8-bit Üst |
|-------------------|--------|--------|--------|-----------|-----------|
| **A register**    | `rax`  | `eax`  |  `ax`  |   `al`    |    `ah`   |
| **B register**    | `rbx`  | `ebx`  |  `bx`  |   `bl`    |    `bh`   |
| **C register**    | `rcx`  | `ecx`  |  `cx`  |   `cl`    |    `ch`   |
| **D register**    | `rdx`  | `edx`  |  `dx`  |   `dl`    |    `dh`   |
| **SI register**   | `rsi`  | `esi`  |  `si`  |   `sil`   |     -     |
| **DI register**   | `rdi`  | `edi`  |  `di`  |   `dil`   |     -     |
| **BP register**   | `rbp`  | `ebp`  |  `bp`  |   `bpl`   |     -     |
| **SP register**   | `rsp`  | `esp`  |  `sp`  |   `spl`   |     -     |

> **Not:** `rsi`, `rdi`, `rbp`, `rsp` register'larında `ah`, `bh` benzeri üst 8-bit parçalar yoktur.

#### Görselleştirme: rbx Register Örneği

```
┌───────────────────────────────────────────────────────────────────┐
│                              rbx (64-bit)                         │
└───────────────────────────────────────────────────────────────────┘
                                |───────────────────────────────────┐
                                │        ebx (32-bit)               │
                                └───────────────────────────────────┘
                                                |───────────────────┐
                                                │   bx (16-bit)     │
                                                └───────────────────┘
                                                        |───────┬───┐
                                                        │  bh   │bl │
                                                        │(8-bit)│(8)│
                                                        └───────┴───┘
```

#### Kullanım Örnekleri

```assembly
; Karakter işlemleri (8-bit)
mov al, [rsi]    ; rax'ın alt 8 biti
mov bl, 'A'      ; rbx'ın alt 8 biti
mov cl, 5        ; rcx'ın alt 8 biti (sayaç)
mov dl, 0        ; rdx'ın alt 8 biti

; 32-bit işlemler  
mov eax, 12345   ; rax'ın alt 32 biti
mov ebx, ecx     ; rbx = rcx (32-bit)

; 16-bit işlemler
mov ax, 100      ; rax'ın alt 16 biti
mov bx, cx       ; rbx = rcx (16-bit)
```

## Korumalı ve Korumasız Register'lar

> x86-64 System V ABI'de bazı register'lar **caller-saved** (çağıran korur), bazıları **callee-saved** (çağrılan korur) olarak tanımlanır. Bu kurallar fonksiyon çağrılarında register değerlerinin korunması için kritiktir.

### Korumasız Register'lar (Caller-Saved / Volatile)
> Bu register'lar fonksiyon çağrısı sırasında **değişebilir**. Fonksiyon çağrısından önce saklanmalıdır.

| Register  | Kullanım Alanı                    | Korunma Sorumluluğu           |
|-----------|-----------------------------------|-------------------------------|
| `rax`     | Dönüş değeri, geçici hesaplamalar |  Çağıran fonksiyon korur      |
| `rcx`     | 4. argüman, geçici hesaplamalar   |  Çağıran fonksiyon korur      |
| `rdx`     | 3. argüman, geçici hesaplamalar   |  Çağıran fonksiyon korur      |
| `rsi`     | 2. argüman                        |  Çağıran fonksiyon korur      |
| `rdi`     | 1. argüman                        |  Çağıran fonksiyon korur      |
| `r8-r11`  | Geçici register'lar               |  Çağıran fonksiyon korur      |

### Korumalı Register'lar (Callee-Saved / Non-Volatile)
> Bu register'lar fonksiyon çağrısı sonrasında **aynı değerde** olmalıdır. Kullanılacaksa fonksiyon başında saklanıp, sonunda geri yüklenmelidir.

| Register  | Kullanım Alanı                | Korunma Sorumluluğu           |
|-----------|-------------------------------|-------------------------------|
| `rbx`     | Genel amaçlı veri saklama     |  Çağrılan fonksiyon korur     |
| `rbp`     | Stack frame pointer           |  Çağrılan fonksiyon korur     |
| `r12-r15` | Genel amaçlı register'lar     |  Çağrılan fonksiyon korur     |

### Özel Register'lar
| Register | Durum                  | Açıklama                        |
|----------|------------------------|---------------------------------|
| `rsp`    |  **Daima korunmalı**   | Stack pointer - değişmemeli     |
| `rip`    |  **Sistem yönetimi**   | Instruction pointer             |

### Pratik Örnek: Register Koruma

```assembly
ft_function:
    ; Korumalı register kullanacaksak sakla
    push rbx        ; rbx korumalı - saklanmalı
    push r12        ; r12 korumalı - saklanmalı
    
    ; İşlemler
    mov rbx, rdi    ; rbx'i güvenle kullan
    mov r12, rsi    ; r12'yi güvenle kullan
    
    call malloc     ; malloc çağrısı - korumasız register'lar değişebilir!
    ; rax, rcx, rdx, rsi, rdi, r8-r11 değişmiş olabilir
    ; ama rbx ve r12 hala güvenli
    
    ; Korumalı register'ları geri yükle
    pop r12         ; r12'yi geri yükle
    pop rbx         ; rbx'i geri yükle
    ret
```

### Kurallar ve İpuçları
| Durum                             | Kural                                         | Örnek                          |
|-----------------------------------|-----------------------------------------------|--------------------------------|
| **Korumasız register kullanımı**  | Fonksiyon çağrısından önce sakla              | `push rax; call func; pop rax` |
| **Korumalı register kullanımı**   | Fonksiyon başında sakla, sonunda geri yükle   | `push rbx; ... ; pop rbx`      |
| **Stack balance**                 | Her push için bir pop, LIFO sırasına dikkat   | -                              |
| **Argüman passing**               | rdi, rsi, rdx, rcx, r8, r9 sırası             | Standart çağrı konvansiyonu    |

### Özet
- 🔴 **Korumasız**: rax, rcx, rdx, rsi, rdi, r8-r11 → Fonksiyon çağrısında değişebilir
- 🟢 **Korumalı**: rbx, rbp, r12-r15 → Fonksiyon çağrısından sonra aynı değerde olmalı  
- 🔒 **Özel**: rsp (stack pointer) → Asla bozulmamalı

**Bu kurallar, farklı fonksiyonların birlikte çalışabilmesi için gerekli protokoldür!**


## 64-bit (x86-64) Assembly Temel Komutları (Intel Syntax)

### Sistem Komutları
| Komut      | Açıklama               |
|------------|------------------------|
| `syscall`  | Sistem çağrısı yapar.  |

### Aritmetik Komutlar
| Komut   | Açıklama                   |
|---------|----------------------------|
| `add`   | İki değeri toplar.         |
| `sub`   | İki değeri çıkarır.        |
| `mul`   | Çarpma işlemi yapar.       |
| `div`   | Bölme işlemi yapar.        |
| `inc`   | Bir değeri artırır.        |
| `dec`   | Bir değeri azaltır.        |

### Bit Düzeyinde Komutlar
| Komut   | Açıklama                   |
|---------|----------------------------|
| `and`   | Bitwise AND.               |
| `or`    | Bitwise OR.                |
| `xor`   | Bitwise XOR.               |
| `not`   | Bitwise NOT.               |

### Kontrol ve Karar Komutları
| Komut   | Açıklama                                   |
|---------|--------------------------------------------|
| `jmp`   | Belirtilen adrese atlar.                   |
| `cmp`   | İki değeri karşılaştırır.                  |
| `je`    | Eşitlik durumunda atlama yapar.            |
| `jne`   | Eşitsizlik durumunda atlama yapar.         |
| `jg`    | Büyükse atlama yapar.                      |
| `jl`    | Küçükse atlama yapar.                      |
| `call`  | Fonksiyonu çağırır.                        |
| `ret`   | Fonksiyondan dönüş yapar.                  |

### Veri Transferi Komutları
| Komut   | Açıklama                                   |
|---------|--------------------------------------------|
| `mov`   | Veriyi bir register'dan diğerine kopyalar. |
| `pop`   | Stack'ten veri çıkarır.                    |
| `push`  | Stack'e veri ekler.                        |
| `test`  | Bir değerin bitlerini kontrol eder.        |

## Flags Register - İşlemci'nin Hafızası

> Assembly'de her karşılaştırma veya aritmetik işlemden sonra, sonuç **Flag register**'ında özel bit'ler olarak saklanır. Jump komutları bu flag'leri okuyarak karar verir.

### Önemli flag'ler

| Flag   | İsim          | Ne Zaman Set Olur            | Örnek                             |
|--------|---------------|------------------------------|-----------------------------------|
| **ZF** | Zero Flag     | Sonuç 0 ise (eşitlik durumu) | `cmp rax, 5` → rax=5 ise ZF=1     |
| **CF** | Carry Flag    | Taşma olursa                 | Unsigned overflow                 |
| **SF** | Sign Flag     | Sonuç negatifse              | `sub rax, rbx` → rax<rbx ise SF=1 |
| **OF** | Overflow Flag | Signed overflow              | Signed işlemlerde taşma           |

### Flag ile Jump Komutları

| Komut      | Kontrol Ettiği Flag     | Anlamı                          | Örnek                  |
|------------|-------------------------|---------------------------------|------------------------|
| `je`/`jz`  | ZF = 1                  | Eşitse/Sıfırsa atla             | `cmp rax, 0` sonrası   |
| `jne`/`jnz`| ZF = 0                  | Eşit değilse/Sıfır değilse atla | Farklıysa git          |
| `jg`       | ZF = 0 ve SF = OF       | Büyükse atla (signed)           | İşaretli büyükse       |
| `jl`       | SF ≠ OF                 | Küçükse atla (signed)           | İşaretli küçükse       |

### Çalışma Mantığı

```assembly
cmp byte [rdi + rax], 0    ; Karşılaştırma yap - FLAGS güncellenir
je .done                   ; ZF flag'ini kontrol et ve karar ver
```

**Adım Adım:**
1. `cmp` komutu iki değeri karşılaştırır (sonucu kaydetmez)
2. FLAGS register'ında ilgili bit'ler set edilir
3. `je` komutu ZF bit'ini okur
4. ZF=1 ise (eşitse) .done'a gider

## Stack - PUSH/POP İşlemleri

> **Stack**, LIFO (Last In, First Out) yapısında çalışan hafıza alanıdır. Assembly'de geçici veri saklama ve register koruma için kullanılır.

### Stack Mantığı

```
Stack
  ↓ PUSH (ekle)
[Değer3]  ← Son eklenen (en üstte)
[Değer2]
[Değer1]  ← İlk eklenen (en altta)
  ↑ POP (çıkar) - Son eklenen ilk çıkar
```

### PUSH ve POP Komutları
| Komut      | İşlev             | Örnek      | Açıklama                          |
|------------|-------------------|------------|-----------------------------------|
| `push rdi` | Stack'e ekle      | `push rdi` | `rdi` değerini stack'e koyar      |
| `pop rax`  | Stack'ten çıkar   | `pop rax`  | Stack'teki son değeri `rax`'a alır|

### Pratik Örnek: ft_strcpy

```assembly
ft_strcpy:
    push rdi        ; 🔒 Orijinal dst adresini sakla
    
.copy_loop:
    mov al, [rsi]   ; Karakter kopyala
    mov [rdi], al   
    inc rsi         ; Kaynak adresini artır
    inc rdi         ; ⚠️ Hedef adresi değişiyor!
    test al, al     
    jnz .copy_loop  
    
    pop rax         ; 🔓 Saklanan orijinal adresi geri al
    ret             ; Orijinal dst adresini döndür
```

### Adım Adım Stack İşlemi

```
BAŞLANGIÇ:
rdi = 0x1000 (dst adresi)

1️⃣ push rdi
Stack:  [0x1000]  ← rsp (stack pointer)
rdi = 0x1000

2️⃣ Döngü boyunca inc rdi  
Stack:  [0x1000]  ← rsp
rdi = 0x1005 (değişti!)

3️⃣ pop rax
Stack:  []
rax = 0x1000 (orijinal adres geri alındı!)
```

### Stack Kullanım Alanları
| Kullanım              | Açıklama                         | Örnek                         |
|-----------------------|----------------------------------|-------------------------------|
| **Register koruma**   | Fonksiyonlarda register saklama  | `push rax; call func; pop rax`|
| **Geçici veri**       | Kısa süreli veri saklama         | Hesaplama sonuçları           |
| **Fonksiyon çağrıları**| Return adresi saklama           | `call` komutu otomatik yapar  |
| **Local değişkenler** | Fonksiyon içi değişkenler        | Stack frame                   |

### Önemli Kurallar

- ⚖️ **Dengeli kullanım**: Her `push` için bir `pop` olmalı
- 📏 **LIFO sırası**: Son eklenen ilk çıkar
- ⚡ **Hızlı erişim**: Stack çok hızlıdır
- 🎯 **rsp register**: Stack pointer, stack'in tepesini gösterir

**Stack = Assembly'nin "geçici hafızası" - unutma ve hatırlama sistemi!**

## Section Türleri

> Section'lar, programın farklı bileşenlerini düzenlemek için kullanılır.

| Section  | Açıklama                                                                                                              |
|----------|-----------------------------------------------------------------------------------------------------------------------|
| `.text`  | Kodların bulunduğu alandır. Çalıştırılabilir kod burada yer alır.                                                     |
| `.data`  | Statik verilerin (değişkenler, sabitler) bulunduğu alandır. Başlangıç değerleri burada tanımlanır.                    |
| `.bss`   | Başlangıç değeri olmayan statik verilerin bulunduğu alandır. Bellekte yer kaplar, ancak dosya boyutuna dahil edilmez. |

## Assembly Label'ları (Etiketler)

> Label'lar assembly kodunda **belirli noktaları işaretleyen adres** etiketleridir. Jump komutlarının hedefi olarak kullanılırlar.

### Label Türleri

| Label Türü       | Örnek          | Açıklama                                           |
|------------------|----------------|----------------------------------------------------|
| **Global Label** | `ft_strlen:`   | Fonksiyon başlangıcı, her yerden erişilebilir      |
| **Local Label**  | `.count_loop:` | Fonksiyon içi etiket, sadece o fonksiyonda geçerli |

### Örnek Kullanım

```assembly
ft_strlen:              ; Global label - fonksiyon başlangıcı
    xor rax, rax        ; rax = 0

.count_loop:            ; Local label - döngü başlangıcı
    cmp byte [rdi + rax], 0  ; Karakteri kontrol et
    je .done            ; Eğer 0 ise .done'a git
    inc rax             ; Sayacı artır
    jmp .count_loop     ; Tekrar döngü başına git

.done:                  ; Local label - bitiş noktası
    ret                 ; Fonksiyonu bitir
```

### Jump Komutları ile Kullanım
| Komut         | Açıklama               | Örnek             |
|---------------|------------------------|-------------------|
| `jmp .label`  | Koşulsuz atlama        | `jmp .count_loop` |
| `je .label`   | Eşitse atlama          | `je .done`        |
| `jne .label`  | Eşit değilse atlama    | `jne .error`      |

### Akış Kontrolü

```
ft_strlen:
    xor rax, rax           # rax = 0
    ↓
.count_loop:
    cmp byte [rdi+rax], 0  # karakter == 0 ?
    ↓              ↓
   ✅ EVET        ❌ HAYIR
    ↓              ↓
   .done:         inc rax     # rax++
   ret            jmp .count_loop  # tekrar döngüye
```

**Label'lar kodda "yer imleri" gibi çalışır - assembly'nin goto sistemi!**

## PIE (Position Independent Executable) Nedir?

> Bu projede bizden derleme sırasında `-no-pie` **kullanmamamız** isteniyor. Peki neden? Bu flag programın güvenli çalışmasını garanti edecek olan PIE yaklaşımını devre dışı bırakır. Bu da güvenlik bakımından risklidir.

- **Amaç: PIE, çalıştırılabilir dosyanın bellekte herhangi bir adrese yüklenebilmesini sağlar; böylece kodun sabit adresleri olmaz.**

- **Nasıl çalışır: Linux'ta PIE ile oluşturulan ELF dosyasının türü DYN olur.** (ELF: Linux yürütülebilir dosya biçimi. DYN: "Dynamic" türünde, taşınabilir adresli dosya.)

- **Kod adreslemeleri RIP-relative veya GOT/PLT üzerinden yapılır.** (RIP: x86-64'te Instruction Pointer. RIP-relative: mevcut konuma göre adresleme. GOT: Global Offset Table — adresleri tutan tablo. PLT: Procedure Linkage Table — fonksiyon çağrısı köprüsü.) 

- **Dinamik bağlayıcı yükleme anında adresleri çözer.** (Dynamic linker: Çalışma anında kütüphane/fonksiyon adreslerini bağlayan bileşen.)

- **Non-PIE farkı: Non-PIE (Type: EXEC) sabit yükleme adresi kullanır; kod adresleri tahmin edilebilir olur ve istismar kolaylaşır.** (EXEC: Sabit konumlu yürütülebilir türü.)

- **Güvenlik etkisi: PIE, ASLR ile birleşince ana yürütülebilir dosyanın başlangıç adresini rastgele yapar; ROP gibi adres-tahmine dayalı saldırıları zorlaştırır.** (ASLR: Address Space Layout Randomization - Bellek bölgelerini her çalıştırmada rastgele yerleştirme. ROP: Return-Oriented Programming - var olan kod parçacıklarıyla saldırı tekniği.)

- **Önemli not: PIE tek başına güvenlik değildir; ASLR kapalıysa veya info leak varsa etkisi azalır.** (Info leak: Bellek adresleri gibi gizli bilgilerin sızması.)

- **Teknik mekanizma:**
    - Aynı dosya içindeki erişimler RIP-relative talimatlarla yapılır.
    - Dış semboller (kütüphane fonksiyonları) GOT/PLT üzerinden dolaylı çağrılır.

- **Performans ve maliyet:**
    - x86-64'te ek maliyet genelde çok küçüktür. (RIP-relative donanımsal olarak hızlıdır.)
    - İlk yüklemede küçük bir gecikme ve dosya boyutunda hafif artış olabilir.
    - Not: 32-bit sistemlerde GOT erişimleri nispeten daha maliyetli olabilir. (Bu proje 64-bit.)

- **Nasıl derlenir / denetlenir:**
    ```bash
    # Derleme (C dosyasını PIE olarak derle)
    gcc -fPIE -O2 main.c -pie -o app
    # -fPIE: Konumdan bağımsız kod üretir
    # -pie: Çıktıyı PIE olarak bağlar
    ```
    ```bash
    # Doğrulama: ELF dosya türünü kontrol et
    readelf -h app | grep Type
    # Çıktı: Type: DYN (PIE ise DYN olmalı)
    ```
    ```bash
    # Doğrulama: Dosya türünü kontrol et
    file app
    # Çıktı: "position independent executable"
    ```

- **Sınırlar: PIE, bellek sızıntılarını veya JIT tabanlı exploitleri tek başına engellemez; ASLR entropisi ve kernel ayarları etkilidir.** (JIT: Just-In-Time compilation — çalışma anında derleme.)


## Derleme ve Çalıştırma

Assembly kodu genellikle bir **assembler** (derleyici) ile derlenir. Örneğin, x86 mimarisi için `nasm` gibi araçlar kullanılır. Derleme sonucu bir **object file** (`.o`) elde edilir.

C ile entegrasyon için şu yapı kullanılır:

1. **Assembly dosyası** (`.s` veya `.asm`) assembler ile derlenir:
   ```
   nasm -f elf64 myfunc.asm -o myfunc.o
   ```
2. **C dosyası** (`.c`) C derleyicisi ile derlenir:
   ```
   gcc -c main.c -o main.o
   ```
3. **Bağlama (Linking):** Her iki object file birleştirilir:
   ```
   gcc main.o myfunc.o -o program
   ```

#### Entegrasyon Yapısı

C kodunda assembly fonksiyonunu şöyle çağırırsın:
```c
// Assembly fonksiyon bildirimi
extern int myfunc(int x);

int main() {
    int result = myfunc(5);
    // ...
}
```

```
📁 KAYNAK DOSYALAR          🔧 DERLEYİCİLER              📦 OBJECT DOSYALAR           🔗 LİNKER                  🚀 SONUÇ
                                                                                                                
┌─────────────────────┐    ┌──────────────┐           ┌─────────────────────┐      ┌──────────────────┐      ┌─────────────────┐
│      main.c         │    │     gcc      │           │      main.o         │      │                  │      │                 │
│ ─────────────────── │──> │ (C Compiler) │──────────>│ ─────────────────── │─────>│                  │      │                 │
│ #include <stdio.h>  │    │              │           │ ┌─────────────────┐ │      │                  │      │     program     │
│ extern int myfunc() │    └──────────────┘           │ │ Machine Code    │ │      │                  │─────>│  (executable)   │
│ int main() {...}    │                               │ │ 0x48 0x89 ...   │ │      │      gcc         │      │                 │
└─────────────────────┘                               │ └─────────────────┘ │      │    (linker)      │      │  ./program      │
                                                      └─────────────────────┘      │                  │      │                 │
┌─────────────────────┐    ┌──────────────┐           ┌─────────────────────┐      │                  │      └─────────────────┘
│     myfunc.s        │    │     nasm     │           │     myfunc.o        │      │                  │
│ ─────────────────── │───>│ (Assembler)  │──────────>│ ─────────────────── │─────>│                  │
│ section .text       │    │              │           │ ┌─────────────────┐ │      │                  │
│ global myfunc       │    └──────────────┘           │ │ Machine Code    │ │      │                  │
│ myfunc: ...         │                               │ │ 0x48 0x31 ...   │ │      │                  │
└─────────────────────┘                               │ └─────────────────┘ │      └──────────────────┘
                                                      └─────────────────────┘
     
📝 AÇIKLAMA:
1️⃣  C dosyası gcc ile compile edilir → machine code (main.o)
2️⃣  Assembly dosyası nasm ile assemble edilir → machine code (myfunc.o)  
3️⃣  İki object dosyası linker tarafından birleştirilir
4️⃣  Tek bir çalıştırılabilir program elde edilir
```

#### Özet

- Assembly kodu assembler ile, C kodu C derleyicisi ile derlenir.
- Object dosyaları linker ile birleştirilir.
- C'den `extern` ile assembly fonksiyonu çağrılır.
- Bu yapı modüler ve yaygın olarak kullanılır.


### Kullanılabilir Komutlar

| Komut                | Açıklama                                            |
|----------------------|-----------------------------------------------------|
| `make`               | Kütüphaneyi derler.                                 |
| `make test`          | Tüm testleri çalıştırır.                            |
| `make test%`         | % yerine <test_adı> yazılır. Dinamik olarak çalışır.|
| `make clean`         | Nesne dosyalarını temizler.                         |
| `make fclean`        | Tüm dosyaları temizler.                             |
| `make re`            | Yeniden derler.                                     |
| `make help`          | Kullanılabilir komutları listeler.                  |

## Fonksiyonlar

- `ft_strlen`: Bir dize uzunluğunu hesaplar.
- `ft_strcpy`: Bir dizeyi başka bir dizeye kopyalar.
- `ft_strcmp`: İki dizeyi karşılaştırır.
- `ft_strdup`: Bir dizeyi kopyalar ve yeni bir bellek alanında saklar.
- `ft_write` : Bir fd'ye veri yazar.
- `ft_read`  : Bir fd'den veri okur.
