# Dokumentacija

Igrac:
- Geometrija: Obični trougao (3 verteksa) koji reaguje na WASD
- Rotacija: Rotira se pomoću A/D tastera sa konstantnom brzinom rotacije
- Kretanje: Pomera se u smeru u kojem je okrenut koristeći trigonometrijske funkcije (`cosf`, `sinf`)
- Vizuelne povratne informacije:
  - Bela boja (normalno stanje)
  - Crveni boja (kada primi udarac)
  - Zelena boja (kada se healuje)

Neprijatelj:
- Spawnu je se svakih 2sec na jednom od 4 ivica ekrana, van ekrana
- Prati igraca
- Normalizacija vektora za AI pracenje
- Obican ispunjeni kvadrat
- Animacija umiranja: Fade-out efekat sa

Laser:
- Krece se u pravcu u kojem je igrac bio okrenut
- Glow efekat: vise slojeva brightnessa crvene boje (4 sloja)
- Ekspanzija: Postepeno se siri u sirinu dok ne dostignu maksimalnu sirinu

Zvezda:
- Krecu se razlicitim brzinama
- Bounce fizika: Odbijanje od ivica ekrana
- Twinkle animacija: Sinusna funkcija za menjanje brightness-a u vremenu
- Dve boje: zuta i bela
- Krug ili pixel (zavisno od velicine)

Planeta:
- Sprite sheet animacija: 50 frame-ova po sprite-u
- Kružno kretanje : Planete se kreću po kružnoj orbiti, koristimo cosinus i sinus za orbitalnu putanju

Particles:
- Particle sistem: Spawnuju se kod uništenja neprijatelja
- Simulacija fizike
- Cestice se krecu u svim smerovima
- Svaka ima odredjenji zivotni vek

Healovi:
- Pojavljuju se posle odredjenog broja poena
- Pulse animacija: Kontinuirano povecanje i smanjenje scale-a (breathing effect)
- Zeleni kvadrati

Health bar:
- Koristimo png za svako srce, padding uzmedju svakog
- Broj srca zavisi od healtha igraca

Kolizija:
- Circle collision: Formula bazirana na rastojanju između centara
- Laser-neprijatelj: spawn cestica, povecanje skora, skreen shake
- Igrac-neprijatelj: heath--,hit timer, screen shake
- Player item: povecanje healtha, heal timer

Screen shake:
- Camera shake: Pomeranje svih elemenata za random offset
- Postepeno smanjenje intenziteta shakea
- Svaki frame random ugao shakea
- Svi elementi sem srca se shakuju

Ostalo:
- Beli flash kada enemy udari playera, lerpovanje izmedju trentune boje i bele
- Crveni overlay pri low healthu: lerpovanje prema crvenoj boji, intezitet zavisi od ostalog healtha
- Vignette: Radijalno tamnjenje od centra ka ivicama
- Gradient: vertikalni gradient od tamnijeg ka svetlijem

Slow motion:
- Time scaling: Množenje deltatime sa slowmo faktorom
- Trigeruju se na slovo F
- Prestaje nakon odredjenog vremena

