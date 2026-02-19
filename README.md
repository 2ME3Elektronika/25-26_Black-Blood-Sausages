# 25-26_Black Blood Sausages

BERARIAZKO KONPROMEZUAK
Kitarra elektrikoarentzat bi efektu pedal sortuko dira, alde batetik, pedal analogiko bat “fuzz” efektuarekin eta bestalde, pedal digital bat (efektu-anitzekoa) 4 efekturekin overdrive, distortsioa, fuzz eta reverb efektuekin.
Gitarra Elektrikorako Fuzz Efektuko
Pedal Analogikoaren Diseinu eta
Fabrikazioaren Txostena
1. Sarrera
Proiektu honen helburua gitarra elektrikorako fuzz efektuko pedal analogiko baten
diseinu, eraikuntza eta balidazioa egitea izan da. Fuzz efektua seinale elektrikoaren
saturazioan oinarritzen da, eta gitarra-seinaleari distortsio nabarmena eragiten dio. Lan
honetan elektronikako oinarrizko kontzeptuak, zirkuitu analogikoen diseinua eta
fabrikazio-prozesu praktikoak aplikatu dira.
2. Proiektuaren plangintza
Hasierako fasean, lan-kontratua definitu zen, eta horrekin batera materialen zerrenda eta
lan-plangintza prestatu ziren. Antolaketa egoki honek proiektuaren faseak zehaztea eta
epeak betetzea ahalbidetu du, baliabideen kudeaketa eraginkorra bermatuz.
3. Aurretiko azterketa teknikoa
Jarraian, aurretik diseinatutako fuzz pedal analogikoen eskemak aztertu ziren, iturri
desberdinetatik lortutako dokumentazioan oinarrituta. Azterketa honen bidez, seinalearen
anplifikazioa, transistoreen saturazioa eta osagaien balioen eragina ulertu ziren, ondorengo
diseinu propioko erabakiak justifikatzeko.
4. Eskema elektrikoaren diseinua
Azterketa teknikoaren ondoren, eskema elektriko propioa diseinatu zen Proteus
softwarearen bidez. Diseinu honetan zirkuituaren egitura osoa zehaztu zen, seinalearen
sarrera, prozesamendua eta irteera barne. Halaber, elikadura-sistema eta
kontrol-elementuak (potentziometroak) integratu ziren.
5. Materialen aukeraketa eta hornidura
Eskema definituta, materialen zerrenda zehatza prestatu zen. Osagaien aukeraketa
funtzionaltasunean, eskuragarritasunean eta kostuan oinarritu zen. Ondoren, beharrezko
osagai guztiak eskatu ziren muntaketa-faseari ekin ahal izateko.
5.1. Material bakoitzaren funtzioa
● Erresistentzia 100 kΩ (50): polarizazio-sareetan eta gain/feedback kontrolaren
inguruan balio tipikoa; inpedantziak eta korronteak finkatzeko.
● Erresistentzia 33 kΩ (50): seinalearen maila eta polarizazio puntua doitzeko
(bias/feedback sareetan erabilia).
● Erresistentzia 1,3 kΩ (50): transistore-etapetan korronteak mugatzeko eta
egonkortzeko (adib. emiter/kollektore sareetan).
● Erresistentzia 1 MΩ (50): sarrera-inpedantzia altua bermatzeko eta
“pull-down/pull-up” moduan klikak eta zaratak murrizteko.
● Kondentsadorea 100 pF (20): maiztasun altuak leuntzeko/egonkortzeko
(anti-oscillation eta zarata murrizketa).
● Kondentsadorea 0,22 µF (100): AC akoplamendua eta maiztasun baxuen pasaera
definitzeko (sarrera/irteera coupling).
● Kondentsadorea 0,033 µF (100): tonu/iragazketa sareetan maiztasun-erantzuna
moldatzeko.
● Kondentsadorea 10 nF (103): iragazki eta akoplamendu osagarriak; maiztasun
ertain-altuen kontrola.
● Kondentsadorea 100 µF (10): elikadura-iragazketa eta egonkortasuna (ripple/zarata
murrizteko).
● Kondentsadorea 22 µF (20): elikadura edo etapa jakinetako bypass/iragazketa
(erantzun egonkorragoa).
● Transistorea BC108B (10): fuzz efektuaren oinarri den anplifikazio/saturazio etapa
sortzeko (distortsio analogikoa).
● Transistorea BD140 (10): irteera edo gidatze etapa sendoagoa egiteko (korronte
handiagoa behar den kasuetan).
● Diodoa 1N4008 (10): polaritate alderantzikatuaren aurkako babesa edo
elikadura-sareko babesa.
● Potentziometro 10 kΩ Lin (2): parametro linealak kontrolatzeko (adib. gain/mix
antzeko doikuntzak, diseinuaren arabera).
● Potentziometro 100 kΩ Audio (1): bolumen/doikuntza subjektiboetarako egokiagoa
(entzumenaren eskala logaritmikoa).
● Potentziometro 1 kΩ Linear (1): doikuntza fin eta laburrak (korronte/erresistentzia
txikiko kontrola).
● Potentziometro 100 kΩ Linear (1): doikuntza zabaleko parametro lineala (adib. bias
edo tonu sare baten kontrola).
● Pultsagailua (10): efektua aktibatu/desaktibatu (footswitch edo kontrol-botoi gisa).
● Bateria (1): elikadura eramangarria emateko (kanpoko elikadurarik gabe).
● Pila (1): elikadura alternatiba (formatuaren arabera: 9V pila, etab.).
● Adaptadorea (bateria) (10): bateria-konexioa eta kableaketa egiteko/ordezkoak.
● Adaptadorea (pila) (1): pilarekin elikatzeko konektore espezifikoa.
● Jack konektorea 6,3 mm (2): audio sarrera (INPUT) eta irteera (OUTPUT) egiteko.
● LED diodoa (1): pedalaren egoera adierazteko (ON/OFF).
6. Protoboard-eko inplementazioa eta balidazioa
PCB-a fabrikatu aurretik, zirkuitua protoboard batean muntatu zen. Fase honen helburua
zirkuituaren portaera egokia zela egiaztatzea izan zen. Proba esperimentalen bidez, fuzz
efektuaren sorrera, seinalearen distortsio-maila eta kontrol-elementuen eragina aztertu
ziren.
7. PCB-aren diseinua eta fabrikazioa
Balidazioaren ondoren, PCB-aren diseinua egin zen Proteus-en, seinalearen bideak
optimizatuz eta zarata minimizatzea kontuan hartuta. Diseinua amaitu ondoren, plaka LPKF
makina erabiliz fresatu zen, diseinu digitaletik pieza fisikora igaroz.
8. Soldadura eta integrazioa
PCB-a prest zegoenean, osagai elektroniko guztiak soldatu ziren. Horrez gain, pedalaren
funtzionamendurako beharrezkoak diren elementu guztiak integratu ziren: pulsagailua,
potentziometroak, elikadura-konektorea eta pilaaren konektorea. Fase honetan soldaduren
kalitatea eta konexioen fidagarritasuna bermatu ziren.
Bestalde, alde batetik kaxa margotu eta sortutako diseinua serigrafitu zen. Elementu guztien
kolokazioa neurtu eta kaxan zuloak egin ziren osagaiak bertan egoki txertatzeko.
228100
63
Wiring a 3PDT switch
(true bypass with LED)
LEDin jack effect out
Teffect in
ground
jumper
out jack
In
T
R
X
Al input del circuito.
Al positivo del circuito. Resistencia LED
Al output del circuito.
ED
Out
Al negativo del circuito.
Al positivo del circuito.
9. Amaierako probak eta funtzionamenduaren
egiaztapena
Azken fasean, gitarra elektrikoa pedalarekin eta anplifikagailuarekin konektatu zen.
Egindako probek erakutsi zuten fuzz efektua behar bezala sortzen zela eta zirkuituak espero
zen portaera zuela, bai soinuen intentsitatean bai egonkortasunean.
10. Ondorioak
Proiektu honen bidez, fuzz efektuko pedal analogiko baten diseinu-prozesu osoa landu da,
ikerketa teorikotik fabrikazio eta probetara arte. Lortutako emaitzek erakusten dute
diseinatutako sistema funtzionala eta egokia dela. Gainera, zirkuitu analogikoen diseinuan
eta fabrikazio-tekniketan ezagutza praktiko baliotsua eskuratu da.
11. Zirkuituaren azalpen elektrikoa (atal teknikoa)
Diseinatutako fuzz efektuko pedal analogikoa transistoreen saturazioan oinarritzen da,
sarrerako seinale sinusoidala modu ez-linealean anplifikatuz eta distortsionatuz. Atal
honetan zirkuituaren funtzionamendu elektrikoa azaltzen da.
11.1. Sarrerako etapa
Gitarra elektrikotik datorren seinalea tentsio txikikoa eta maiztasun-tarte zabalekoa da.
Seinale hori zirkuitura sartzean, kondentsadore baten bidez akoplatzen da, DC osagaia
blokeatzeko eta AC seinalea bakarrik pasatzeko. Etapa honetan, erresistentzia batek
sarrerako inpedantzia definitzen du, gitarra-pastillen karga murrizteko.
11.2. Anplifikazio eta saturazio etapa
Zirkuituaren oinarria transistore bidezko anplifikazio-etapa da. Transistorea
aktibo-eremuan lan egitera polarizatzen da, baina sarrerako seinalearen anplitudea
handitzean, mozketa eta saturazio egoeretara iristen da. Fenomeno horren ondorioz,
seinalearen gailurrak mozten dira (clipping), eta horrek fuzz efektuaren ezaugarri den
distortsioa sortzen du.
Anplifikazio-maila eta distortsioaren intentsitatea erresistentzien eta potentziometroen
balioen bidez kontrolatzen dira, erabiltzaileari soinuaren izaera aldatzeko aukera emanez.
11.3. Seinalearen kontrola eta tonua
Zirkuituan txertatutako potentziometroek irteerako seinalearen maila eta distortsioaren
intentsitatea doitzea ahalbidetzen dute. Gainera, kondentsadore eta erresistentzien
konbinazioek maiztasun jakin batzuk indartu edo ahuldu ditzakete, fuzz efektuaren
tonalitatea aldatuz.
11.4. Irteerako etapa
Anplifikazio-etaparen ondoren, seinalea irteerako kondentsadore baten bidez bidaltzen da
anplifikagailura. Kondentsadore honek berriro ere DC osagaia ezabatzen du, irteerako
ekipoa babestuz eta seinale garbiagoa bermatuz. Irteerako inpedantzia egokitu egiten da,
seinalearen galera minimizatzeko.
11.5. Elikadura-sistema
Pedalak tentsio baxuko DC elikadura erabiltzen du, bai pila baten bidez, bai kanpoko
elikadura-iturri baten bidez. Elikadura-zirkuituan iragazki-kondentsadoreak erabili dira
tentsioaren fluktuazioak eta zarata elektrikoa murrizteko, zirkuituaren egonkortasuna
hobetuz.
11.6. Ondorio teknikoa
Azaldutako egitura elektrikoari esker, diseinatutako zirkuituak fuzz efektu klasiko baten
portaera lortzen du. Diseinuak sinpletasuna eta eraginkortasuna uztartzen ditu, eta
transistoreen ezaugarri ez-linealak modu kontrolatuan aprobetxatzen ditu soinuaren
distortsioa sortzeko.
12. Hardwarearen bloke-diagrama (pedal analogikoa)

Pedal analogikoa:
● Interneten aurretik eginda dauden eskemak aztertuko dira.
● Eskema propioa diseinatuko da proteus-en.
● Materialen zerrenda osatuko da eta eskaera egingo litzateke beharrezkoa izango balitz.
● Protoboard-ean muntaia egingo da, pedalaren funtzionamendua ziurtatzeko.
● Proteus-en PCB-aren diseinua egin.
● LPKF makinan fresatu.
● Plaka ongi dagoela egiaztatzen denean, osagai elektroniko guztiak soldatuko dira bertan.
● Plaka, pultsagailua, potentziometroak eta elikadura eta pilaren konektoreak egoki txertatuko dira pedalaren kaxan diseinua guztiz bukatuta uzteko.
● Kitarra pedalarekin anplifikagailu batera konektatu eta efektua egoki entzun beharko da 15 minutuz jarraian, parametro aldaketak eginez denbora tarte horretan.

Pedal analogikorako material zerrenda:
OSAGAIAK	KANTITATEA
Erresistentzia 100K	50
Erresistentzia 33K	50
Erresistentzia 1K3	50
Erresistentzia 1M	50
Kondentsadorea 100pF	20
Kondentsadorea 0,22uF	100
Kondentsadorea 0,033uF	100
Kondentsadorea 10 nF	103
Kondentsadorea 100uF	10
Kondentsadorea 22 uF	20
Transistor BC108B	10
Transistor BD140	10
Diode 1N4008	10
Potentziometro 10k Lin	2
Potentziometro 100k Audio	1
Potentziometro 1k Linear	1
Potentziometro 100k Linear	1
Pultsadorea	10
Bateria 	1
Pila	1
Adaptadorea (bateria)	10
Adaptadorea (pila)	1
Jack konektorea (6.3mm)	2
LED diodoa	1

9 Efektuko Pedal Digital Multiefektuaren
Diseinu eta Inplementazioa
1. Sarrera
Proiektu honen helburua gitarra elektrikorako pedal digital multiefektu bat garatzea izan
da, efektu hauekin: Delay, Reverb, Chorus, Flanger, Phaser, Autowah, Distortsioa,
Tremolo eta Vibrato. Sistemak Teensy 4.0 + Audio Shield plataforma erabiltzen du
audioaren sarrera/irteera kudeatzeko, eta erabiltzaile-interfazea osatzeko LCD I2C
displaya, encoder rotatiboa, potentziometroa eta pultsagailua integratzen dira. Gailuak
aurikularretarako irteera dauka eta 5V DC-tan funtzionatzen du.
2. Aurretiko ikerketa eta erreferentzien azterketa
Lehenengo fasean, interneten informazio tekniko eta adibide praktiko ugari bilatu ziren:
antzeko multiefektu-proiektuak, audio digitalaren oinarrizko kontzeptuak, hardware aukera
posibleak eta kontrol-interfaze tipikoak. Aurkitutako erreferentziak aztertuta, funtsezko
erabakiak hartu ziren: erabiliko den mikrocontroller plataforma, audio sarrera/irteera modua
eta erabiltzaile-interfazearen egitura.
3. Sistema hautaketa eta arkitektura orokorra
Ikerketaren ondoren, Teensy 4.0 hautatu zen prozesamendu-unitate gisa, eta Audio Shield
audio-kodeketarako/deskodeketarako (ADC/DAC) eta audioaren kudeaketarako. Hautaketa
honen arrazoia izan zen:
● DSP aplikazioetarako egokia den errendimendua
● Audio sarrera/irteera interfaze erosoa
● Garapen-ingurunean (Arduino IDE) integrazio erraza
4. Garapen-ingurunearen prestaketa
Garapena abiatzeko, Arduino IDE-n beharrezkoak ziren:
● eguneratzeak
● liburutegi espezifikoak
● eta plataformaren euskarria
instalatu ziren. Honek konpilazioa, kargaketa eta proba iteratiboak modu egonkorrean egitea
ahalbidetu zuen.
5. Materialen zerrenda
Proiektuaren garapenerako beharrezkoak izan diren osagai elektroniko eta mekanikoak
jarraian zehazten dira. Materialen aukeraketa funtzionaltasunean, bateragarritasunean eta
erabilgarritasunean oinarritu da.
Osagaia Kantitatea
Protoboard 1
Teensy 4.0 1
Audio Shield 1
Potentziometroak (10 kΩ) 3
Encoder motako
potentziometroa
1
Pultsagailuak 4
LCD I2C displaya 1
Jack konektoreak 2
Jack / Minijack adaptadoreak 2
Micro SD txartela 1
Kaxa mekanikoa (hardware) 1
Erresistentziak (10 kΩ) 4
Elikadura-adaptadorea (bateria) 1
Bateria 1
Material-zerrenda honi esker, sistemaren hardwarearen muntaketa, audio-sarrera/irteera,
kontrol-interfazea eta elikadura modu egokian garatu ahal izan dira.
5.1. Material bakoitzaren funtzioa
● Protoboard (1): zirkuitua behin-behinean muntatzeko eta prototipoa azkar
probatzeko.
● Teensy 4.0 (1): prozesamendu-unitatea; audio DSP algoritmoak (9 efektuak)
exekutatzen ditu.
● Audio Shield (1): ADC/DAC bidez audioa digitalizatu eta berriro analogikora
bihurtzen du; audio I/O kudeatzen du.
● Potentziometroak 10 kΩ (3): erabiltzaileak parametro analogikoak doitzeko (adib.
mix, rate, feedback…).
● Potentziometro encoder (1): menu/efektu aukeraketa eta parametroen nabigazioa
(inkrementu/dekrementu).
● Pultsagailuak (4): efektua aktibatu/desaktibatu, menu-funtzioak edo preset/aldaketa
azkarrak egiteko.
● LCD I2C displaya (1): efektu aktiboa eta parametroen balioak bistaratzen ditu.
● Jack konektoreak (2): audio sarrera eta irteera nagusia (gitarra ↔ pedal ↔
anplifikagailua).
● Jack/Minijack adaptadoreak (2): konektibitatea egokitzeko (adib. aurikularrak edo
bestelako kableak).
● Micro SD txartela (1): audio-fitxategiak, presetak edo konfigurazioak
gordetzeko/irakurtzeko (erabileraren arabera).
● Kaxa mekanikoa (1): osagai guztiak babestu eta pedal-formatuko erabilera
mekanikoa ahalbidetzen du.
● Erresistentziak 10 kΩ (4): pull-up/pull-down, zatiketa edo sarrera egonkortzeko
(kontrol eta seinale osagarrietan).
● Elikadura-adaptadorea (bateria) (1): elikadura-konexioa eta egokitzapena (5V
sistemarako).
● Bateria (1): elikadura eramangarria emateko (kanpoko elikadurarik gabe
erabiltzeko).
11.6. Hardwarearen bloke-diagrama
6. Softwarearen garapena
Efektu-sistemaren logika eta kontrola C++-n garatu ziren, Arduino IDE erabiliz.
Softwarearen helburu nagusiak izan ziren:
● audio-seinalea denbora errealean prozesatzea
● 9 efektuen artean aukeraketa eta konfigurazioa ahalbidetzea
● erabiltzaile-interfazea (display + encoder + pot + botoia) integratzea
● irteera (anplifikagailua / aurikularrak) modu fidagarrian ematea
7. Prototipoa, kargaketa eta balidazioa
Kodea amaitutakoan, prototipoaren muntaia egin zen eta programa Teensy-ra kargatu
zen. Ondoren, proba funtzionalak egin ziren:
● efektu bakoitzaren aktibazioa eta aldaketa egiaztatuz
● kontrolen erantzuna eta erabiltzaile-esperientzia aztertuz
● audioaren egonkortasuna (etenik/zaratarik gabe) bermatuz
8. Integrazioa eta muntaketa fisikoa
Behin prototipoa balidatuta, elementu guztiak egoki integratu ziren:
● plaka eta konektoreak
● pultsagailua(k), potentziometroa eta encoderra
● LCD I2C displaya
● elikadura eta pilaren/iturriaren konektoreak
● aurikularren irteera
Fase honetan, kableaketa eta konexioen fidagarritasuna eta mekanikoki sendo egotea
zaindu zen.
Bestalde, alde batetik kaxa margotu eta sortutako diseinua biniloan inprimitu zen, kaxan
itsasteko. Elementu guztien kolokazioa neurtu eta kaxan zuloak egin ziren osagaiak bertan
egoki txertatzeko.
9. Amaierako probak
Azkenik, gitarra pedalean sartu eta anplifikagailuarekin (eta behar denean aurikularrekin)
probatu zen. Egiaztatu zen:
● efektuen soinua koherentea eta erabilgarria dela
● aldaketa azkarrak eta kontrolen erantzuna egokia dela
● 5V elikadurarekin sistema egonkorra dela
10. Ondorioak
Proiektuak erakutsi du posible dela multiefektu digital funtzional bat garatzea plataforma
txiki batekin, hardware eta softwarearen integrazio egokia eginez. Emaitza gisa, 9 efektu
dituen pedal erabilgarria lortu da, kontrol-interfaze praktikoarekin eta irteera anizkoitzarekin
(anplifikagailua/aurikularrak). Gainera, DSP kontzeptuetan, embedded programazioan eta
prototipatzean esperientzia esanguratsua eskuratu da.
11. Zirkuituaren eta sistemaren azalpen
teknikoa
11.1. Arkitektura: seinalearen fluxua (Audio Path)
Sistemaren funtzionamendua honela laburbil daiteke:
1. Gitarra sarrera (analogikoa): gitarra-seinalea tentsio txikikoa da eta inpedantzia
kontuan hartzea garrantzitsua da (sarrerak ez dezan pastilla gehiegi kargatu).
2. Audio Shield-eko ADC: seinale analogikoa digitalizatu egiten da
(laginketa-maiztasuna eta bit-sakonera sistemak definitzen dituen parametroen
arabera).
3. DSP prozesamendua Teensy-n: seinale digitalari efektuak aplikatzen zaizkio
denbora errealean.
4. Audio Shield-eko DAC: prozesatutako seinalea berriro analogikora bihurtzen da.
5. Irteera analogikoa: anplifikagailura edo aurikular-irteerara bidaltzen da (irteeraren
maila kontrolatuz eta distortsio digitala/klipping-a saihestuz).
11.2. Kontrol-sistema eta erabiltzaile-interfazea
Erabiltzaile-interfazeak efektuen aukeraketa eta parametroen doikuntza ahalbidetzen du:
● Encoder rotatiboa: normalean efektuen artean nabigatzeko edo parametro bat
hautatzeko erabiltzen da (biraketa: +/-, sakatzea edo botoiarekin konbinatuta:
“enter/confirm” logika).
● Potentziometroa: parametro jarraitu baterako sarrera analogikoa (adib.
mix/feedback/rate/depth).
● Pultsagailua: efektua aktibatu/desaktibatu, preset aldaketa edo menu-funtzio sinple
bat.
● LCD I2C displaya: egoera erakusteko (efektuaren izena, parametro balioa,
aktibo/inaktibo).
Interfazeak, praktikan, egoera-makina (state machine) moduko logika jarraitzen du: efektu
hautaketa → parametro aukeraketa → parametro doikuntza → aplikazioa.
11.3. Efektuen oinarri DSP azalpena
● Delay: seinalearen kopia bat buffer batean gordetzen da eta denbora jakin baten
ondoren irteeran gehitzen da. Parametro tipikoak: delay time, feedback, mix.
● Reverb: espazio-sentsazioa simulatzen du, atzerapen anitz eta iragazketa erabiliz.
Parametroak: room size/decay, mix.
● Chorus: delay oso labur bat + modulazioa (LFO) erabiliz, “bikoiztutako” sentsazioa
sortzen du. Parametroak: rate, depth, mix.
● Flanger: chorusaren antzekoa baina delay denbora oso txikia eta feedback
handiagoa erabiliz; “sweep” metalikoa sortzen du.
● Phaser: seinalea fasean desplazatzen duten iragazki-kateak erabiliz; LFO batek
desplazamendua mugitzen du.
● Autowah: seinalearen anplitudeak (envelope) iragazki baten maiztasun zentralari
eragiten dio; “wah” automatikoa.
● Distortsioa: saturazio digitala edo “waveshaping” teknikak erabiliz; garrantzitsua da
irteeran klipping desatsegina ez sortzea.
● Tremolo: seinalearen anplitudea LFO batekin modulatu (bolumen-uhina).
● Vibrato: pitch/denbora txikiko modulazioa (delay labur modulatu baten bidez),
chorus-etik desberdinduz (normalean dry signal gabe edo gutxiago).
11.4. Denbora errealeko baldintzak eta egonkortasuna
Efektu guztiak denbora errealean exekutatzen direnez, sistemak honako hauek bermatu
behar ditu:
● latentzia txikia (jo eta entzun artean atzerapen handirik ez)
● prozesamendu jarraitua buffer underrun/overrun gabe
● parametro aldaketak “klik” edo artefaktu handirik sortu gabe (mugimenduen leuntzea
erabil daiteke)
11.5. Elikadura eta konexioak (5V)
Gailuak 5V DC-tan elikatzen da. Diseinuan kontuan hartu beharreko puntuak:
● elikadura egonkorra (zarata gutxikoa) audio-kalitatea ez kaltetzeko
● masa-konexioak eta kableaketa ondo antolatzea (hum/zarata minimizatzeko)
● aurikular-irteeraren erabileran, irteerako maila eta inpedantzia egokiak mantentzea,
distortsio edo gainkarga saihestuz

Pedal digitala:
● Interneten ahalik eta informazio gehien bilatuko da, aurretik eginda dauden proiektuak aurkitu eta analizatuko dira.
● Erabiliko den sistema hautatu (kasu honetan teensy 4.0 erabiliko da AudioShield-arekin).
● Arduino IDE-n kodea sortzeko beharrezkoak diren eguneratze eta liburutegi guztiak instalatuko dira.
● Beharrezkoak diren materialen zerrenda idatzi eta eskura ez dauden materialen eskaera egin.
● C++ hizkuntzan kodea sortuko da Arduino IDE softwarea erabiliaz.
● Kodea bukatuta prototipoaren muntaia egingo da eta Teensy-an kodea kargatuko da.
● Prototipoaren probak egin eta funtzionamendu egokia egiaztatu.
● Display-a, plaka, pultsagailuak, potentziometroak eta elikadura eta pilaren konektoreak egoki txertatuko dira pedalaren kaxan diseinua guztiz bukatuta uzteko.
● Azkenik, kitarra pedalarekin anplifikagailu batera konektatu eta efektuaren soinu egokia egiaztatu beharko da, 15 minutuz jarraian egoki funtzionatu beharko du, efektu eta parametro aldaketak eginez denbora tarte horretan.

Pedal digitalerako material zerrenda:
OSAGAIAK	KANTITATEA
Protoboard	1
Tensy 4.0	1
AudioShield 	1
Potentziometroak (10k ohm)	3
Potentziometro encoder	1
Pultsagailuak	4
Display LCD i2c	1
Jack konektoreak	2
Jack/minijack adaptadorea	2
SD txartela (micro sd)	1
Kaxa (hardware)	1
Erresistentziak(10K)	4
Adaptadorea (bateria)	1
Bateria	1



