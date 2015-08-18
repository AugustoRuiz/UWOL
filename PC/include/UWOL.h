#pragma once

enum TipoEnemigo
{
	Wolfy = 0,
    Franky = 1,
    Vampy = 2, 
    Fanty = 3
};

enum Velocidad
{
    Lento = 0,
    Rapido = 1
};

enum Direccion
{
    Horizontal = 0,
	Vertical = 1
};

enum TilesFondo
{
    Tile0 = 0,
    Tile1 = 1,
    Tile2 = 2,
    Tile3 = 3
};

enum TilePlataforma
{
    Plat0 = 0,
    Plat1 = 1,
    Plat2 = 2,
    Plat3 = 3,
    Plat4 = 4,
    Plat5 = 5,
    Plat6 = 6,
    Plat7 = 7
};

enum Estados
{
	Normal = 1,
	Desnudo = 2,
	Parpadeo = 4,
	Muriendo = 8,
	TodasMonedasCogidas = 16,
	SalidaIzq = 32,
	SalidaDer = 64,
	Muerto = 128
};

enum Facing
{
	Left = 1,
	Right = 2
};

enum ActionKeys
{
	ActionKeysUp = 1,
	ActionKeysDown = 2,
	ActionKeysLeft = 3,
	ActionKeysRight = 4
};

enum ControlMode
{
	Keyboard = 1,
	Joystick = 2
};