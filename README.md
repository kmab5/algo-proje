# [Piano Tiles](https://github.com/kmab5/algo-proje)

## Grup Üyeleri

- Mohammad Nasim Muhtasebzada (Öğr. No. 24253810) ([Github](https://github.com/nasim-01))
- Samuel Worash Getaneh (Öğr. No. 24253809) ([Github](https://github.com/kmab))
- Santiago Ntutumu Mansogo Mikue (Öğr. No. 24253811) ([Github](https://github.com/santi-mikue))

---

## 1. Oyun Fikri

**Piano Tiles**, popüler mobil oyundan esinlenilmiş bir ritim oyunudur. Oyuncular ekranda yukarıdan aşağıya düşen siyah karoları doğru zamanda ve doğru tuşla (A, S, K, L) vurmaya çalışır. Oyun üç zorluk seviyesinden oluşur:

- **EASY (Kolay):** İlk 50 karo – yavaş hız
- **MEDIUM (Orta):** 51-100 arası karolar – orta hız
- **HARD (Zor):** 100+ karo – hızlı tempo

Oyuncular, karoya ne kadar doğru zamanda vururlarsa o kadar yüksek puan alır:

- **Perfect (Mükemmel):** 10 puan – karonun tam ortasına yakın
- **Great (Harika):** 6 puan – iyi zamanlama
- **Good (İyi):** 3 puan – kabul edilebilir zamanlama

Karo ekranın dışına çıkarsa veya yanlış tuşa basılırsa oyun sona erer.

### Oyun Modları

Oyun farklı ses modları ile oynanabilir:

| Mod | Açıklama |
| --- | -------- |
| **RANDOM** | Her karoya tıklandığında rastgele piyano notası çalar |
| **SONGONLY** | Arka planda müzik çalar (varsayılan mod) |
| **NOSOUND** | Sessiz mod – ses efektleri kapalı |

### Ses ve Müzik Özellikleri

- **Piyano Notaları:** C4, D4, E4, F4, G4, A4, B4 notaları karoya tıklandığında çalar (RANDOM modunda)
- **Arka Plan Müziği:** Oyun sırasında müzik çalar (SONGONLY modunda)
- **Hata Sesi:** Yanlış tuşa basıldığında özel ses efekti (Gb4)

### Görsel Özellikler

- **Arka Plan Görseli:** Özelleştirilebilir arka plan resmi
- **Pulse Animasyonları:** Tuşlara basıldığında renkli görsel geri bildirim
- **Başarı Bildirimleri:** Good, Great, Perfect yazıları animasyonlu olarak gösterilir

---

## 2. Derleme ve Çalıştırma

### Gereksinimler

- **Raylib** kütüphanesi (C:/raylib/raylib dizininde kurulu olmalı)
- **MinGW** veya **w64devkit** derleyicisi
- Windows işletim sistemi

### Kaynak Dosyaları (resources/ klasörü)

Oyunun çalışması için aşağıdaki dosyaların `resources/` klasöründe bulunması gerekir:

| Dosya | Açıklama |
| ----- | -------- |
| `highscore.dat` | Yüksek skor verileri |
| `party.png` | Arka plan görseli |
| `dancemonkey.mp3` | Arka plan müziği |
| `C4.mp3`, `D4.mp3`, `E4.mp3`, `F4.mp3`, `G4.mp3`, `A4.mp3`, `B4.mp3` | Piyano notaları |
| `Gb4.mp3` | Hata sesi |

### Derleme Adımları

#### Debug Modu (Hata ayıklama için)

```bash
mingw32-make.exe RAYLIB_PATH=C:/raylib/raylib PROJECT_NAME=main OBJS=main.c BUILD_MODE=DEBUG
```

#### Release Modu

```bash
mingw32-make.exe RAYLIB_PATH=C:/raylib/raylib PROJECT_NAME=main OBJS=main.c
```

### Çalıştırma

Derleme başarılı olduktan sonra oluşan `main.exe` dosyasını çalıştırın:

```bash
./main.exe
```

### Kontroller

| Tuş | İşlev |
| ----- | ------- |
| A | 1. sütun (sol) |
| S | 2. sütun |
| K | 3. sütun |
| L | 4. sütun (sağ) |
| SPACE | Oyunu duraklat/devam et |
| R | Oyunu yeniden başlat (Game Over ekranında) |

---

## 3. İşaretçi (Pointer) Kullanımı

Projede işaretçiler aşağıdaki amaçlarla kullanılmıştır:

### 3.1. Oyun Durumu Yönetimi (`Game *game`)

Tüm fonksiyonlar `Game` yapısına işaretçi alarak çalışır. Bu sayede:

- `GameInit(Game *game)` – Oyunu başlatır
- `UpdateGame(Game *game)` – Oyun fiziğini günceller
- `DrawFrame(Game *game)` – Ekranı çizer
- `GameEnd(Game *game)` – Oyunu sonlandırır
- `CheckTile(Game *game, int col)` – Karo kontrolü yapar

**Amaç:** Büyük yapıyı kopyalamak yerine referans ile geçirerek bellek tasarrufu sağlamak ve değişikliklerin orijinal yapıya yansımasını garantilemek.

### 3.2. Dosya İşlemleri (`FILE *file`)

`loadScores()` ve `saveScores()` fonksiyonlarında dosya işaretçisi kullanılır:

```c
FILE *file;
file = fopen(HIGHSCORE_FILE, "r");
```

**Amaç:** Dosya okuma/yazma işlemlerini yönetmek.

### 3.3. Dizi Sıralama (`int* arr`)

`BubbleSort(int* arr, int len)` fonksiyonunda dizi işaretçisi kullanılır:

```c
static void BubbleSort(int* arr, int len) {
    // Diziyi yerinde sıralar
}
```

**Amaç:** Diziyi kopyalamadan doğrudan orijinal dizi üzerinde sıralama yapmak.

---

## 4. Yüksek Skor Dosya Formatı

Yüksek skorlar `resources/highscore.dat` dosyasında saklanır.

### Format Özellikleri

- **Dosya türü:** Düz metin (plain text)
- **Yapı:** Her satırda bir skor
- **Kayıt sayısı:** En fazla 10 skor
- **Sıralama:** Büyükten küçüğe (en yüksek skor en üstte)

### Örnek `highscore.dat` İçeriği

```txt
150
120
95
80
65
50
40
30
20
10
```

### Dosya İşlemleri

- **Okuma:** `fscanf(file, "%d", &score)` ile satır satır okunur
- **Yazma:** `fprintf(file, "%d\n", game->scores[i])` ile her skor yeni satıra yazılır
- **Güncelleme:** Yeni skor en düşük skordan yüksekse, listeye eklenir ve Bubble Sort ile sıralanır

---

## 5. Ekipteki Görev Paylaşımı

| Öğrenci | Ana Görevler |
| --------- | ---------- |
| **Santiago** | Dosya işlemleri (yüksek skor kaydetme/yükleme), Bubble Sort algoritması |
| **Samuel** | Oyun mantığı, zorluk seviyeleri tasarımı, puan hesaplama sistemi, ses sistemi |
| **Nasim** | Proje yapılandırması ve klasör düzeni, kullanıcı girdisi yönetimi, arka plan görseli |

> **Not:** Proje grup çalışması olarak yürütülmüştür. Her üye birbirinin koduna katkıda bulunmuş, kod incelemesi yapmış ve geliştirme sürecinde aktif rol almıştır. Yukarıdaki tablo yalnızca ana sorumluluk alanlarını göstermektedir.

### Ortak Çalışmalar

- Kod incelemesi ve hata ayıklama
- Test ve optimizasyon
- Dokümantasyon hazırlama
- Fikir paylaşımı ve problem çözme

---

## 6. Oyun Gösterimi

![Oyun Gösterimi](oyun.gif)

### made with 💖 for algo lab 121
