==============================================
Switch 6 Porte Remoto – IK1APW
==============================================
Autore: Emanuele Rossi - IK1APW
Versione: Finale con connessione remota
Hardware: Arduino Nano + Display OLED SSD1306 + Encoder + EEPROM

----------------------------------------------
1. DESCRIZIONE
----------------------------------------------
Il sistema consente di controllare 6 porte digitali tramite:
 - Encoder rotativo con pulsante (controllo locale)
 - Comunicazione seriale con software remoto (Python o .exe)

Tutte le impostazioni vengono salvate automaticamente in EEPROM
e vengono ripristinate al riavvio.

----------------------------------------------
2. CONNESSIONI HARDWARE
----------------------------------------------
OLED 0.96" SSD1306  -->  SDA=A4, SCL=A5 (I2C addr 0x3C)
Encoder CLK          -->  D3
Encoder DT           -->  D4
Encoder SW           -->  D5
Porte digitali 1–6   -->  D6–D11 (uscite logiche 0/1)
Alimentazione        -->  5V / GND

----------------------------------------------
3. COMUNICAZIONE SERIALE
----------------------------------------------
- Baud rate: 9600
- Terminatore riga: \n
- Esempi di comandi inviabili via terminale o software remoto:

   ON1 ... ON6   → accende la porta 1–6
   OFF1 ... OFF6 → spegne la porta 1–6
   STATUS        → restituisce lo stato di tutte le porte
   CLEAR         → resetta le memorie (EEPROM)

Risposta STATUS esempio:
STATE:1,0,0,1,0,1

----------------------------------------------
4. INSTALLAZIONE
----------------------------------------------
1. Aprire il file "Switch_6_Porte_Remoto_IK1APW.ino" con Arduino IDE.
2. Selezionare la scheda "Arduino Nano" e la porta COM corretta.
3. Caricare il programma.
4. Collegare il dispositivo e verificare il funzionamento locale e remoto.

----------------------------------------------
5. NOTE
----------------------------------------------
- Tutte le modifiche allo stato delle porte sono salvate in EEPROM.
- Al riavvio, le porte vengono ripristinate all'ultimo stato salvato.
- Il display OLED mostra sempre lo stato aggiornato e la selezione corrente.

----------------------------------------------
IK1APW © 2025 - Tutti i diritti riservati
----------------------------------------------
