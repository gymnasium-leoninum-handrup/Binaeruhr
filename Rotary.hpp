#ifndef __ROTARY
#define __ROTARY

// Reihenfolgen   Status
// 11               0
// 01               1
// 00               2
// 10   Rechts      3 mit Ausgabe

// 11
// 10               4
// 00               5
// 01               6
// 11   Links       7  mit Ausgabe

#define Start 0
#define R1 1
#define R2 2
#define R3 3
#define L1 4
#define L2 5
#define L3 6
#define rechts 8
#define links 9

class RotaryEncoder {
private:
    const byte Automat[4][7] = {
        { Start, R2, R2, Start, L2, L2, Start },
        { R1, R1, Start, Start, Start, L3, L3 },
        { L1, Start, R3, R3, L1, Start, Start },
        { Start, Start, Start, rechts, Start, Start, links },
    };
    byte _vrp;
    byte _arp;
    char _ergebnis;
    int _count;
    byte _pinclk, _pindt, _pintast;
    byte _status;
    byte _statustaste;

public:
    RotaryEncoder(byte _Pinclk, byte _Pindt, byte _Pintast)
    {
        pinMode(_Pinclk, INPUT);
        pinMode(_Pindt, INPUT);
        pinMode(_Pintast, INPUT);
        digitalWrite(_Pinclk, HIGH);
        digitalWrite(_Pindt, HIGH);
        digitalWrite(_Pintast, HIGH);
        _vrp = 10;
        _arp = 11;
        _ergebnis = 0;
        _count = 0;
        _pinclk = _Pinclk;
        _pindt = _Pindt;
        _pintast = _Pintast;
        _status = 0;
        _statustaste = false;
    }

    int poll()
    {
        int _ergebnis;
        _arp = (digitalRead(_pindt) << 1 | digitalRead(_pinclk));
        Serial.print("arp: ");
        Serial.println(_arp);
        if (!(_arp == _vrp)) {
            _vrp = _arp;
            _status = Automat[_arp][_status];
            Serial.println(_status);
            switch (_status) {
            case rechts: {
                _ergebnis = 1;
                _status = 0;
                _count++;
                break;
            }
            case links: {
                _ergebnis = -1;
                _status = 0;
                _count--;
                break;
            }
            default:
                _ergebnis = 0;
            }
        }

        Serial.println(_ergebnis);
        return _ergebnis;
    }

    int getCount()
    {
        return _count;
    }

    void setCount(byte count)
    {
        _count = count;
    }

    byte getTaste()
    {
        byte _taste;
        byte zaehle;
        zaehle = 0;
        _taste = digitalRead(_pintast);
        if (!(_taste == _statustaste)) {
            zaehle = _statustaste;
            _statustaste = _taste;
            delay(500); //entprellen
            while (digitalRead(_pintast) == LOW) {
                zaehle++;
            }
        }

        return zaehle;
    }
};

#endif
