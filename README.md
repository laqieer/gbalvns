# gbalvns
Leaf Visual Novel System for GBA

VN/AVG/ADV/Gal game engine based on [sizuku_gba2](http://akkera102.sakura.ne.jp/gbadev/index.php?NO.100%20sizuku_gba2).

## Demo

[Download](https://github.com/laqieer/sizuku/releases)

### Screenshot

![](https://media.discordapp.net/attachments/682141375587680274/820353613573652481/summer-2.png)![](https://media.discordapp.net/attachments/682141375587680274/820353609774530580/summer-0.png)![](https://media.discordapp.net/attachments/682141375587680274/820353612844498994/summer-1.png)![](https://media.discordapp.net/attachments/682141375587680274/820353615485992960/summer-3.png)

### Credit

フリー素材:

- Background: きまぐれアフター
- Character: キャラクターなんとか機
- Music: 魔王魂

## Feature

Create your own games on GBA without coding! No programming skills are required!

- Story branching
- Save & load at any point
- Skip to the next branch point & return to the previous branch point freely
- Environment configuration
- Logging

## Prerequisite

[devkitPro](https://devkitpro.org/)

## Build

1. Build `tool`: Build [wav28ad](https://github.com/laqieer/wav28ad) and copy it to `tool`
1. Build `core`: `cd core && make`
1. Build `asset`: `cd asset && gbfs asset.gbfs script/*.dat image/*.img sound/*.8ad`
1. Link `core` & `asset`: `cat core/core.gba asset/asset.gbfs > summer.gba`

## Doc

Messages, events and animations are written in Script Markup Language.

### Message & Event

`asset/script/`

[sizuku_gba2's doc](http://akkera102.sakura.ne.jp/gbadev/index.php?Doc.15%20PC%A5%B2%A1%BC%A5%E0%A1%D6%BC%B6%A1%D7%A4%CE%B0%DC%BF%A2%CA%FD%CB%A1%281%29)

### Animation

`core/res/`

`core/anime.c`

