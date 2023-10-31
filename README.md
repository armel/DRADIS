# DRADIS
![basic](https://img.shields.io/badge/M5Stack-BASIC-blue)
![grey](https://img.shields.io/badge/M5Stack-GREY-blue)
![fire](https://img.shields.io/badge/M5Stack-FIRE-orange)
![core2](https://img.shields.io/badge/M5Stack-CORE2-green)
![aws](https://img.shields.io/badge/M5Stack-AWS-orange)
![cores3](https://img.shields.io/badge/M5Stack-CORES3-purple)

![licence](https://img.shields.io/github/license/armel/HAL9000)
![language](https://img.shields.io/github/languages/top/armel/HAL9000)
![size](https://img.shields.io/github/repo-size/armel/HAL9000)
![version](https://img.shields.io/github/v/release/armel/HAL9000)
![activity](https://img.shields.io/github/commit-activity/y/armel/HAL9000)

**Many thanks to them and all my [donors](#donations)🙏🏻** 

This project is a little [DRADIS](https://en.battlestarwikiclone.org/wiki/DRADIS) simulator on M5Stack. Use cases ? None ! It's just for fun 😎 Note that this project works best with the M5GO/M5GO2 module (if you want to take advantage of the Cylon eye scanner...).

# Usage

Use left and right buttons to adjust brightness. Use the middle button to hear [Cylon Eye Scanner](https://www.youtube.com/watch?v=dv3dUc_G4a0) and change theme (Grey, Blue, Sepia).

# Compilation

First, edit `platformio.ini` file and change `default_envs` target :

- `default_envs = core` for Core, 
- `default_envs = core2` for Core2
- `default_envs = cores3` for Core S3.

You can compile the source code and flash your M5Stack.

Remember to upload the FileSystem Image too (the `/data` folder content videos, images and wav). If you don't know how to upload the FileSystem Image, take a look at this [tutorial](https://www.donskytech.com/how-to-upload-filesystem-image-in-platformio-ide/).

# Clock feature (only on ![fire](https://img.shields.io/badge/M5Stack-FIRE-orange) ![core2](https://img.shields.io/badge/M5Stack-CORE2-green) ![aws](https://img.shields.io/badge/M5Stack-AWS-orange) ![cores3](https://img.shields.io/badge/M5Stack-CORES3-purple))

If you're using an M5Stack with __PSRAM memory__, you can activate a clock. Prepare a FAT32 formated micro SD card. Edit `DRADIS.ini and copy it at the root of the micro SD card.

This is what the `DRADIS.ini` file looks like:

```
; Wifi Config
[wifi]
wifi_ssid = YOUR_SSID
wifi_password = YOUR_PASSWORD

; Time Config
[time]
time_server = pool.ntp.org
time_timezone = CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00
```

So, complete your `wifi_ssid` and `wifi_password`. Change `time_server` if you want. And finally, specify your `time_timezone`. 

> Note: the default timezone is France (Europe/Paris). If you need to adapt the timezone, consult this more detailed [list](https://github.com/blindsidenetworks/bigbluebutton-1/blob/master/bbb-voice-conference/config/freeswitch/conf/autoload_configs/timezones.conf.xml). Contact me if you need help or if you live in an exotic region... Nevertheless, here are a few examples of values:

| Country           | Timezone             | 
| -------------- | -------------------- |
| Europe/Paris | CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00 |
| Europe/Berlin | CET-1CEST,M3.5.0,M10.5.0/3 |
| Europe/Brussels | CET-1CEST,M3.5.0,M10.5.0/3 |
| Europe/Bucharest | EET-2EEST,M3.5.0/3,M10.5.0/4 |
| Europe/London | GMT0BST,M3.5.0/1,M10.5.0 |
| Europe/Sofia | EET-2EEST,M3.5.0/3,M10.5.0/4 |
| America/Los_Angeles | PST8PDT,M3.2.0,M11.1.0 |
| America/New_York | EST5EDT,M3.2.0,M11.1.0 |
| America/Martinique | AST4 |
| America/Montreal | EST5EDT,M3.2.0,M11.1.0 |
| America/Guadeloupe | AST4 |
| Australia/Brisbane | EST-10 |
| Asia/Hong_Kong | HKT-8 |
| Asia/Tokyo | JST-9 |
| Pacific/Noumea | NCT-11 |


When your M5Stack starts up, if your `DRADIS.ini` file is present at the root of your micro SD card and if it is valid, you will see the time displayed. 


# Credits
 
Many thanks to...

| Project             | Author                                                |  Link                                           |
|:------------------- | :---------------------------------------------------- | :---------------------------------------------- |
| DRADIS stuffs     | David Gian-Cursio and Matt Haley                                                   | https://www.gian-cursio.net/2007/02/dradis-icons/ |
| M5Unified           | [Lovyan03](https://twitter.com/lovyan03)              | https://github.com/m5stack/M5Unified            |
| JPEGDEC             | Larry Bank                                            | https://github.com/bitbank2/JPEGDEC             |
| FastLED             | FastLED                                               | https://github.com/FastLED/FastLED              |

Battlestar Galactica and all related objects are the property of Universal.

This project was created for purely recreational and geek purposes and MUST NOT be used in any project which will result in financial gain through their use. By the way, attempts to make money though use of this project will likely result in legal action courtesy of Universal, and won't exactly make you my favorite person, either. YOU HAVE BEEN WARNED.

# Donations

Special thanks to Rolf Schroeder DL8BAG, Brian Garber WB8AM, Matt B-Wilkinson M6VWM, Robert Agnew KD0TVP, Meinhard Frank Günther DL0CN, Johan Hansson SM0TSC, Tadeusz Pater VA7CPM, Frederic Ulmer F4ESO, Joshua Murray M0JMO, Mark Hammond N8MH, Angel Mateu Muzzio EA4GIG (2 times 🍷🍷), Hiroshi Sasaki JL7KGW, Robert John Williams VK3IE, Mark Bumstead M0IAX, Félix Symann F1VEO, Patrick Ruhl DG2YRP, Michael Beck DH5DAX, Philippe Nicolas F4IQP, Timothy Nustad KD9KHZ, Martin Blanz DL9SAD, Edmund Thompson AE4TQ, Gregory Kiyoi KN6RUQ, Patrick Samson F6GWE, George Kokolakis SV3QUP, Ambrose "Bo" Barry W4GHV, Roger Bouche F1HCN, Christopher Platt, Pascal Paquet F4ICR, Gregory Kiyoi, Ning Yang BH7JAG, Mitsuhiko Nagasawa JL1LYT, Mike Mann G4GOC, David Cappello, Matt Brinkhoff KB0RXC, Franklin Beider WD9GZ, Robrecht Laurens ON4ROB, Florian Wolters DF2ET, James Gatwood WA9JG, Christoph Gässler DL6SEZ, Roger Kenyon WB2YOJ, Jean-pierre Billat F1RXM (2 times 🍷🍷), John Sheppard G4WOD, Jean-Cyrille Vercollier F6IWW and Kenneth Goins Jr for their donations. That’s so kind of them. Thanks so much 🙏🏻

If you find this project fun and useful then [buy me a glass of wine](https://www.paypal.me/F4HWN) 🍷 🤗 

~~You could use the code F4HWN in order to get 5% discount on the [M5Stack shop](https://shop.m5stack.com/?ref=LUxetaH4) 🎁~~

By the way, you can follow me on [Twitter](https://twitter.com/F4HWN) and post pictures of your installation with your M5Stack. It always makes me happy ;) 
