<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.10" tiledversion="1.11.0" name="Level1" tilewidth="16" tileheight="16" tilecount="150" columns="15">
 <image source="../Textures/Levels/Level1Tiles.png" width="240" height="160"/>
 <tile id="2">
  <properties>
   <property name="Interactable" value="Brick"/>
  </properties>
 </tile>
 <tile id="3">
  <properties>
   <property name="Interactable" value="LuckyBlock"/>
  </properties>
  <animation>
   <frame tileid="3" duration="300"/>
   <frame tileid="18" duration="300"/>
   <frame tileid="33" duration="300"/>
  </animation>
 </tile>
</tileset>
