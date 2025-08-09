# libasm

- [Genel Bakış](#genel-bakış)
- [64-bit (x86-64) Register'lar ve Görevleri](#64-bit-x86-64-registerlar-ve-görevleri)
- [64-bit (x86-64) Assembly Temel Komutları (Intel Syntax)](#64-bit-x86-64-assembly-temel-komutları-intel-syntax)
- [Flags Register - İşlemci'nin Hafızası](#flags-register---i̇şlemcinin-hafızası)
- [Stack - PUSH/POP İşlemleri](#stack---pushpop-i̇şlemleri)
- [Section Türleri](#section-türleri)
- [Assembly Label'ları (Etiketler)](#assembly-labelları-etiketler)
- [Derleme ve Çalıştırma](#derleme-ve-çalıştırma)
- [Kullanılabilir Komutlar](#kullanılabilir-komutlar)
- [Fonksiyonlar](#fonksiyonlar)

## Genel Bakış
>  **libasm**, dize manipülasyonu için basit ve hızlı bir assembly kütüphanesidir.

>  Tüm işlemler **register**'lar üzerinde gerçekleşir; fonksiyon argümanları ve dönüş değerleri doğrudan register'lar ile yönetilir.

>  Doğru register seçimi, kodun optimizasyonu için kritik öneme sahiptir.

>  Register'lar, CPU'nun ultra hızlı hafıza birimleridir; **RAM'den 100-200 kat daha hızlı** fakat sayıca sınırlıdır.

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

| Register | 64-bit | 32-bit | 16-bit | 8-bit Alt | 8-bit Üst |
|----------|--------|--------|--------|-----------|-----------|
| **A register** | `rax` | `eax` | `ax` | `al` | `ah` |
| **B register** | `rbx` | `ebx` | `bx` | `bl` | `bh` |
| **C register** | `rcx` | `ecx` | `cx` | `cl` | `ch` |
| **D register** | `rdx` | `edx` | `dx` | `dl` | `dh` |
| **SI register** | `rsi` | `esi` | `si` | `sil` | - |
| **DI register** | `rdi` | `edi` | `di` | `dil` | - |
| **BP register** | `rbp` | `ebp` | `bp` | `bpl` | - |
| **SP register** | `rsp` | `esp` | `sp` | `spl` | - |

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

| Flag   | İsim | Ne Zaman Set Olur | Örnek |
|--------|------|-------------------|-------|
| **ZF** | Zero Flag | Sonuç 0 ise (eşitlik durumu) | `cmp rax, 5` → rax=5 ise ZF=1 |
| **CF** | Carry Flag | Taşma olursa | Unsigned overflow |
| **SF** | Sign Flag | Sonuç negatifse | `sub rax, rbx` → rax<rbx ise SF=1 |
| **OF** | Overflow Flag | Signed overflow | Signed işlemlerde taşma |

### Flag ile Jump Komutları

| Komut | Kontrol Ettiği Flag | Anlamı | Örnek |
|-------|---------------------|--------|-------|
| `je`/`jz` | ZF = 1 | Jump if Equal/Zero | `cmp rax, 0` sonrası |
| `jne`/`jnz` | ZF = 0 | Jump if Not Equal/Not Zero | Farklıysa git |
| `jg` | ZF=0 AND SF=OF | Jump if Greater | İşaretli büyükse |
| `jl` | SF ≠ OF | Jump if Less | İşaretli küçükse |

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
Stack = Tabak Yığını
  ↓ PUSH (ekle)
[Değer3]  ← Son eklenen (en üstte)
[Değer2]
[Değer1]  ← İlk eklenen (en altta)
  ↑ POP (çıkar) - Son eklenen ilk çıkar
```

### PUSH ve POP Komutları

| Komut | İşlev | Örnek | Açıklama |
|-------|-------|-------|----------|
| `push rdi` | Stack'e ekle | `push rdi` | rdi'nin değerini stack'e koy |
| `pop rax` | Stack'ten çıkar | `pop rax` | Stack'teki son değeri al, rax'a koy |

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

| Kullanım | Açıklama | Örnek |
|----------|----------|-------|
| **Register koruma** | Fonksiyonlarda register'ları sakla | `push rax; call func; pop rax` |
| **Geçici veri** | Kısa süreli veri saklama | Hesaplama sonuçları |
| **Fonksiyon çağrıları** | Return adresi saklama | `call` komutu otomatik yapar |
| **Local değişkenler** | Fonksiyon içi değişkenler | Stack frame |

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

| Komut | Açıklama | Örnek |
|-------|----------|-------|
| `jmp .label` | Koşulsuz sıçrama | `jmp .count_loop` |
| `je .label` | Eşitse sıçrama | `je .done` |
| `jne .label` | Eşit değilse sıçrama | `jne .error` |

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


## Derleme ve Çalıştırma

Assembly kodu genellikle bir **assembler** (derleyici) ile derlenir. Örneğin, x86 mimarisi için `nasm`, `gas` (GNU Assembler) gibi araçlar kullanılır. Derleme sonucu bir **object file** (`.o`) elde edilir.

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

