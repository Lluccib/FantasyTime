
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    return ret;
}

bool Map::Start() {

    //Calls the functon to load the map, make sure that the filename is assigned
    SString mapPath = path;
    mapPath += name;
    bool ret = Load(mapPath);

    //#inicializa el pathfinding
    pathfinding = new PathFinding();
    
    //#inicializa el navigation map

    /*uchar* navigationMap = NULL;
    CreateNavigationMap(mapData.width, mapData.height, &navigationMap);
    pathfinding->SetNavigationMap((uint)mapData.width, (uint)mapData.height, navigationMap);
    RELEASE_ARRAY(navigationMap);*/

    return ret;
}

bool Map::Update(float dt)
{
    if(mapLoaded == false)
        return false;

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && mapLayerItem->data->properties.GetProperty("Draw")->value) {

            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    int gid = mapLayerItem->data->Get(x, y);
                    TileSet* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    app->render->DrawTexture(tileset->texture,
                        pos.x,
                        pos.y,
                        &r);
                }
            }
        }
        mapLayerItem = mapLayerItem->next;

    }

    return true;
}

bool Map::LoadColliders(pugi::xml_node& layerNode) {
    bool ret = true;

    // Iterar sobre los tiles de la capa "Collisions"
    pugi::xml_node dataNode = layerNode.child("data");
    if (dataNode) {
        int tileIndex = 0;
        for (pugi::xml_node tileNode = dataNode.child("tile"); tileNode; tileNode = tileNode.next_sibling("tile")) {
            int gid = tileNode.attribute("gid").as_int();

            // Verificar si el tile representa un collider
            //gid == 1 -> rojo
            //gid == 2 -> verde
            //gid == 3 -> azul
            //gid == 4 -> amarillo
            if (gid == 9182) {
                // Calcular las coordenadas del tile en el mundo del juego
                int x = tileIndex % mapData.width;
                int y = tileIndex / mapData.width;

                // Crear un collider para el tile rojo en la posici�n (x, y)
                PhysBody* collider = app->physics->CreateRectangle(x * mapData.tileWidth + 16.1f, y * mapData.tileHeight + 16, mapData.tileWidth, mapData.tileHeight, STATIC);
                collider->ctype = ColliderType::PLATFORM; // Define el tipo de collider seg�n tu necesidad
            }
            else if (gid == 9183) {
                // Calcular las coordenadas del tile en el mundo del juego
                int x = tileIndex % mapData.width;
                int y = tileIndex / mapData.width;

                // Crear un collider para el tile rojo en la posici�n (x, y)
                PhysBody* collider = app->physics->CreateRectangle(x * mapData.tileWidth + 16, y * mapData.tileHeight + 16, mapData.tileWidth, mapData.tileHeight, STATIC);
                collider->ctype = ColliderType::ENEMY; // Define el tipo de collider seg�n tu necesidad
            }
            else if (gid == 9184) {
                // Calcular las coordenadas del tile en el mundo del juego
                int x = tileIndex % mapData.width;
                int y = tileIndex / mapData.width;

                // Crear un collider para el tile rojo en la posici�n (x, y)
                PhysBody* collider = app->physics->CreateRectangle(x * mapData.tileWidth + 16, y * mapData.tileHeight + 16, mapData.tileWidth + 1, mapData.tileHeight, STATIC);
                collider->ctype = ColliderType::WALL; // Define el tipo de collider seg�n tu necesidad
            }
            else if (gid == 9185) {
                // Calcular las coordenadas del tile en el mundo del juego
                int x = tileIndex % mapData.width;
                int y = tileIndex / mapData.width;

                // Crear un collider para el tile rojo en la posici�n (x, y)
                PhysBody* collider = app->physics->CreateRectangle(x * mapData.tileWidth + 16, y * mapData.tileHeight + 32, mapData.tileWidth, mapData.tileHeight , STATIC);
                collider->ctype = ColliderType::PLATFORM; // Define el tipo de collider seg�n tu necesidad
            }
            else if (gid == 9186) {
                // Calcular las coordenadas del tile en el mundo del juego
                int x = tileIndex % mapData.width;
                int y = tileIndex / mapData.width;

                // Crear un collider para el tile rojo en la posici�n (x, y)
                PhysBody* collider = app->physics->CreateRectangle(x * mapData.tileWidth + 16, y * mapData.tileHeight + 1, mapData.tileWidth, 2, STATIC);
                collider->ctype = ColliderType::PLATFORM; // Define el tipo de collider seg�n tu necesidad
            }
            else if (gid == 9187) {
                // Calcular las coordenadas del tile en el mundo del juego
                int x = tileIndex % mapData.width;
                int y = tileIndex / mapData.width;

                // Crear un collider para el tile rojo en la posici�n (x, y)
                PhysBody* collider = app->physics->CreateRectangle(x * mapData.tileWidth + 30, y * mapData.tileHeight + 1, mapData.tileWidth / 2, 2, STATIC);
                collider->ctype = ColliderType::PLATFORM; // Define el tipo de collider seg�n tu necesidad
            }

            tileIndex++;
        }
    }
    else {
        ret = false;
    }

    return ret;
}
iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;

    return ret;
}

iPoint Map::WorldToMap(int x, int y) 
{
    iPoint ret(0, 0);

    //

    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}

TileSet* Map::GetTilesetFromTileId(int gid) const
{
    ListItem<TileSet*>* item = mapData.tilesets.start;
    TileSet* set = NULL;

    while (item)
    {
        set = item->data;
        if (gid < (item->data->firstgid + item->data->tilecount))
        {
            break;
        }
        item = item->next;
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.Clear();

    // Remove all layers
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.maplayers.start;

    while (layerItem != NULL)
    {
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }

    return true;
}

// Load new map
bool Map::Load(SString mapFileName)
{
    bool ret = true;

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName.GetString(), result.description());
        ret = false;
    }

    if(ret == true)
    {
        ret = LoadMap(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadAllLayers(mapFileXML.child("map"));
    }
    
    // NOTE: Later you have to create a function here to load and create the colliders from the map

    //PhysBody* c1 = app->physics->CreateRectangle(224 + 128, 543 + 32, 256, 64, STATIC);
    //c1->ctype = ColliderType::PLATFORM;//CUADRADO 1

    //PhysBody* c2 = app->physics->CreateRectangle(352 + 64, 384 + 32, 128, 64, STATIC);
    //c2->ctype = ColliderType::PLATFORM;//CUADRADO 2

    //PhysBody* c3 = app->physics->CreateRectangle(256, 704 + 32, 576, 64, STATIC);
    //c3->ctype = ColliderType::PLATFORM;

    //PhysBody* c4 = app->physics->CreateRectangle(20*20, 704 + 32, 576, 64, STATIC);
    //c3->ctype = ColliderType::PLATFORM;


    //el cuadrado de colision se pone en el centro, debemos a�adirle la mitad de cada cordenada tanto en x como en y para que se situe en su lugar correspondiente.

   
    if(ret == true)
    {
        LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
        LOG("width : %d height : %d",mapData.width,mapData.height);
        LOG("tile_width : %d tile_height : %d",mapData.tileWidth, mapData.tileHeight);
        
        LOG("Tilesets----");

        ListItem<TileSet*>* tileset;
        tileset = mapData.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d",tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }

        LOG("Layers----");

        ListItem<MapLayer*>* mapLayer;
        mapLayer = mapData.maplayers.start;

        while (mapLayer != NULL) {
            LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
            mapLayer = mapLayer->next;
        }
    }

    if(mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

bool Map::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        //Load map general properties
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
        mapData.type = MAPTYPE_UNKNOWN;
    }

    return ret;
}

bool Map::LoadTileSet(pugi::xml_node mapFile){

    bool ret = true; 

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet* set = new TileSet();

        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        SString texPath = path; 
        texPath += tileset.child("image").attribute("source").as_string();
        set->texture = app->tex->Load(texPath.GetString());

        mapData.tilesets.Add(set);
    }

    return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
    bool ret = true;

    //Load the attributes
    layer->id = node.attribute("id").as_int();
    layer->name = node.attribute("name").as_string();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();

    LoadProperties(node, layer->properties);

    //Reserve the memory for the data 
    layer->data = new uint[layer->width * layer->height];
    memset(layer->data, 0, layer->width * layer->height);

    //Iterate over all the tiles and assign the values
    pugi::xml_node tile;
    int i = 0;
    for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
    {
        layer->data[i] = tile.attribute("gid").as_int();
        i++;
    }

    return ret;
}

// L06: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    // L09: DONE 3: Get the screen coordinates of tile positions for isometric maps 
    if (mapData.orientation == MapOrientation::ORTOGRAPHIC) {
        ret.x = x * mapData.tilewidth;
        ret.y = y * mapData.tileheight;
    }

    if (mapData.orientation == MapOrientation::ISOMETRIC) {
        ret.x = x * mapData.tilewidth / 2 - y * mapData.tilewidth / 2;
        ret.y = x * mapData.tileheight / 2 + y * mapData.tileheight / 2;
    }

    return ret;
}

bool Map::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
    {
        //Load the layer
        MapLayer* mapLayer = new MapLayer();
        ret = LoadLayer(layerNode, mapLayer);

        //add the layer to the map
        mapData.maplayers.Add(mapLayer);

        //add collisions
        std::string layerName = layerNode.attribute("name").as_string();
        if (layerName == "Collisions") {
            ret = LoadColliders(layerNode);
        }
    }

    return ret;
}
// L08: DONE 7: Implement a method to get the value of a custom property
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.list.Add(p);
    }

    return ret;
}
// L08: DONE 7: Implement a method to get the value of a custom property
Properties::Property* Properties::GetProperty(const char* name)
{
    ListItem<Property*>* item = list.start;
    Property* p = NULL;

    while (item)
    {
        if (item->data->name == name) {
            p = item->data;
            break;
        }
        item = item->next;
    }

    return p;
}

// L09: DONE 5: Add method WorldToMap to obtain  map coordinates from screen coordinates 
iPoint Map::WorldToMap(int x, int y) {

    iPoint ret(0, 0);

    if (mapData.orientation == MapOrientation::ORTOGRAPHIC) {
        ret.x = x / mapData.tilewidth;
        ret.y = y / mapData.tileheight;
    }

    if (mapData.orientation == MapOrientation::ISOMETRIC) {
        float half_width = mapData.tilewidth / 2;
        float half_height = mapData.tileheight / 2;
        ret.x = int((x / half_width + y / half_height) / 2);
        ret.y = int((y / half_height - (x / half_width)) / 2);
    }

    return ret;
}

int Map::GetTileWidth() {
    return mapData.tilewidth;
}

int Map::GetTileHeight() {
    return mapData.tileheight;
}

// L13: Create navigationMap map for pathfinding
void Map::CreateNavigationMap(int& width, int& height, uchar** buffer) const
{
    bool ret = false;

    //Sets the size of the map. The navigation map is a unidimensional array 
    uchar* navigationMap = new uchar[navigationLayer->width * navigationLayer->height];
    //reserves the memory for the navigation map
    memset(navigationMap, 1, navigationLayer->width * navigationLayer->height);

    for (int x = 0; x < mapData.width; x++)
    {
        for (int y = 0; y < mapData.height; y++)
        {
            //i is the index of x,y coordinate in a unidimensional array that represents the navigation map
            int i = (y * navigationLayer->width) + x;

            //Gets the gid of the map in the navigation layer
            int gid = navigationLayer->Get(x, y);

            //If the gid is a blockedGid is an area that I cannot navigate, so is set in the navigation map as 0, all the other areas can be navigated
            //!!!! make sure that you assign blockedGid according to your map
            if (gid == blockedGid) navigationMap[i] = 0;
            else navigationMap[i] = 1;
        }
    }

    *buffer = navigationMap;
    width = mapData.width;
    height = mapData.height;


