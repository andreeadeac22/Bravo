#include "TerrainTile.h"

#include <osg/Material>

#include <osgUtil/SmoothingVisitor>
#include <osgUtil/Simplifier>

using namespace osg;

TerrainTile::TerrainTile(float t_width, Vec2s coords)
    : tile_position(new PositionAttitudeTransform()),
      tile_geode(new Geode()), tile_width(t_width),
      tile_coords(coords), generated(false)
{
    this->show();
}

void TerrainTile::setPosition(osg::Vec3d pos)
{
    tile_position->setPosition(pos);
}

void TerrainTile::show()
{
    if (!tile_position->containsNode(tile_geode)) {
        tile_position->addChild(tile_geode);
    }
}

void TerrainTile::hide()
{
    if (tile_position->containsNode(tile_geode)) {
        tile_position->removeChild(tile_geode);
    }
}

void TerrainTile::setModel(Node* geom)
{
    //Remove any current nodes
    int numChildren = tile_geode->getNumChildren();
    if (numChildren > 0)
        tile_geode->removeChildren(0, numChildren);

    //Add the next model
    if (geom != nullptr) {
        tile_geode->addChild(geom);
    }
}

/** Class WaterTile **/

WaterTile::WaterTile(float t_width, osg::Vec2s coords)
    : TerrainTile(t_width, coords), geometry(new Geometry())
{
    ref_ptr<Material> material = new Material;
    material->setColorMode(Material::DIFFUSE);
    material->setAmbient(Material::FRONT_AND_BACK, Vec4(0.0, 0.0, 0.1, 1));
    material->setDiffuse(Material::FRONT_AND_BACK, Vec4(0.0, 0.0, 0.6, 1.0));
    material->setSpecular(Material::FRONT_AND_BACK, Vec4(0.2, 0.2, 0.8, 1));
    material->setShininess(Material::FRONT_AND_BACK, 64);
    tile_geode->getOrCreateStateSet()->setAttributeAndModes(material.get(), StateAttribute::ON);

    Vec3Array* triangles = new Vec3Array;

    //Lower triangle
    triangles->push_back(Vec3(0.0f, 0.0f, 0.0f));
    triangles->push_back(Vec3(tile_width, 0.0f, 0.0f));
    triangles->push_back(Vec3(0.0f, tile_width, 0.0f));

    //Upper
    triangles->push_back(Vec3(tile_width, 0.0f, 0.0f));
    triangles->push_back(Vec3(tile_width, tile_width, 0.0f));
    triangles->push_back(Vec3(0.0f, tile_width, 0.0f));

    geometry->setVertexArray(triangles);
    geometry->addPrimitiveSet(new DrawArrays(PrimitiveSet::TRIANGLES, 0, 6));

    tile_geode->addChild(geometry.get());

    osgUtil::SmoothingVisitor smoothifier;
    smoothifier.apply(*tile_geode.get());
}

BoringIceTile::BoringIceTile(float t_width, Vec2s coords)
    : TerrainTile(t_width, coords), geometry(new Geometry())
{
    ref_ptr<Material> material = new Material;
    material->setColorMode(Material::DIFFUSE);
    material->setAmbient(Material::FRONT_AND_BACK, Vec4(0.1, 0.1, 0.1, 1));
    material->setDiffuse(Material::FRONT_AND_BACK, Vec4(0.6, 0.6, 0.6, 1.0));
    material->setSpecular(Material::FRONT_AND_BACK, Vec4(0.8, 0.8, 0.8, 1));
    material->setShininess(Material::FRONT_AND_BACK, 64);
    tile_geode->getOrCreateStateSet()->setAttributeAndModes(material.get(), StateAttribute::ON);

    Vec3Array* triangles = new Vec3Array;

    float iceHeight = 20.0f;

    //Lower triangle
    triangles->push_back(Vec3(0.0f, 0.0f, iceHeight));
    triangles->push_back(Vec3(tile_width, 0.0f, iceHeight));
    triangles->push_back(Vec3(0.0f, tile_width, iceHeight));

    //Upper
    triangles->push_back(Vec3(tile_width, 0.0f, iceHeight));
    triangles->push_back(Vec3(tile_width, tile_width, iceHeight));
    triangles->push_back(Vec3(0.0f, tile_width, iceHeight));

    geometry->setVertexArray(triangles);
    geometry->addPrimitiveSet(new DrawArrays(PrimitiveSet::TRIANGLES, 0, 6));

    tile_geode->addChild(geometry.get());

    osgUtil::SmoothingVisitor smoothifier;
    smoothifier.apply(*tile_geode.get());
}

StaticBoundaryIceTile::StaticBoundaryIceTile(float t_width, osg::Vec2s coords)
    : TerrainTile(t_width, coords)
{
}

void StaticBoundaryIceTile::setHeightMap(HeightMap *heightMap)
{
    geomFull = new Group;
    geomReduced = new Group;

    render(heightMap, geomFull);

    HeightMapSimplifier heightSimpl(heightMap, 8);
    render(&heightSimpl, geomReduced);
}

void StaticBoundaryIceTile::updateEyeDist(float dist)
{
    float lowres = 1500.0f;
    float cutoff = 6000.0f;

    if (dist < lowres) {
        setModel(geomFull.get());
    } else if (dist < cutoff) {
        setModel(geomReduced.get());
    } else {
        setModel(nullptr);
    }
}

void StaticBoundaryIceTile::render(IHeightMap *heightMap, osg::ref_ptr<osg::Group> geometry)
{
    ref_ptr<Material> iceMaterial = new Material;
    iceMaterial->setColorMode(Material::DIFFUSE);
    iceMaterial->setAmbient(Material::FRONT_AND_BACK, Vec4(0.1, 0.1, 0.1, 1));
    iceMaterial->setDiffuse(Material::FRONT_AND_BACK, Vec4(0.6, 0.6, 0.6, 1.0));
    iceMaterial->setSpecular(Material::FRONT_AND_BACK, Vec4(0.8, 0.8, 0.8, 1));
    iceMaterial->setShininess(Material::FRONT_AND_BACK, 64);

    ref_ptr<Material> waterMaterial = new Material;
    waterMaterial->setColorMode(Material::DIFFUSE);
    waterMaterial->setAmbient(Material::FRONT_AND_BACK, Vec4(0.0, 0.0, 0.1, 1));
    waterMaterial->setDiffuse(Material::FRONT_AND_BACK, Vec4(0.0, 0.0, 0.6, 1.0));
    waterMaterial->setSpecular(Material::FRONT_AND_BACK, Vec4(0.2, 0.2, 0.8, 1));
    waterMaterial->setShininess(Material::FRONT_AND_BACK, 64);

    //tile_geode->getOrCreateStateSet()->setAttributeAndModes(material.get(), StateAttribute::ON);

    Geometry* waterSurface = new Geometry;
    waterSurface->getOrCreateStateSet()->setAttributeAndModes(waterMaterial.get(), StateAttribute::ON);

    Vec3Array* triangles = new Vec3Array;

    //Lower triangle
    triangles->push_back(Vec3(0.0f, 0.0f, 0.0f));
    triangles->push_back(Vec3(tile_width, 0.0f, 0.0f));
    triangles->push_back(Vec3(0.0f, tile_width, 0.0f));

    //Upper
    triangles->push_back(Vec3(tile_width, 0.0f, 0.0f));
    triangles->push_back(Vec3(tile_width, tile_width, 0.0f));
    triangles->push_back(Vec3(0.0f, tile_width, 0.0f));

    waterSurface->setVertexArray(triangles);
    waterSurface->addPrimitiveSet(new DrawArrays(PrimitiveSet::TRIANGLES, 0, 6));

    Vec3Array* iceFaceTris = new Vec3Array;
    int iceFaceTriCount = 0;

    int vertexCount = heightMap->getWidth() + 1;

    Vec3Array* iceUpperVertices = new Vec3Array;
    Vec3Array* iceUpperNorms = new Vec3Array;
    for (int x = 0; x < vertexCount; x++) {
        for (int y = 0; y < vertexCount; y++) {
            iceUpperVertices->push_back(heightMap->getVertex(x, y));
            iceUpperNorms->push_back(heightMap->getNormal(x, y));
        }
    }

    Geometry* iceUpperSurface = new Geometry;
    iceUpperSurface->getOrCreateStateSet()->setAttributeAndModes(iceMaterial.get(), StateAttribute::ON);

    iceUpperSurface->setVertexArray(iceUpperVertices);
    iceUpperSurface->setNormalArray(iceUpperNorms, Array::BIND_PER_VERTEX);

    DrawElementsUInt* iceUpperFaces = new DrawElementsUInt(PrimitiveSet::TRIANGLES, 0);

    for (int x = 0; x < vertexCount - 1; x++) {
        for (int y = 0; y < vertexCount - 1; y++) {
            bool tl = heightMap->isIce(x, y + 1);
            bool tr = heightMap->isIce(x + 1, y + 1);
            bool bl = heightMap->isIce(x, y);
            bool br = heightMap->isIce(x + 1, y);

            uint32_t ibl = x * vertexCount + y;
            uint32_t ibr = (x + 1) * vertexCount + y;
            uint32_t itl = x * vertexCount + (y + 1);
            uint32_t itr = (x + 1) * vertexCount + (y + 1);

            Vec3 vbl = heightMap->getVertex(x, y);
            Vec3 vbr = heightMap->getVertex(x + 1, y);
            Vec3 vtl = heightMap->getVertex(x, y + 1);
            Vec3 vtr = heightMap->getVertex(x + 1, y + 1);

            //If the vertices at bottom are ice
            if (bl && br) {
                if (tl) {
                    //Forms a left hand right hand triangle
                    iceUpperFaces->push_back(ibl);
                    iceUpperFaces->push_back(ibr);
                    iceUpperFaces->push_back(itl);

                    if (!tr) {
                        //Draw diagonal face of ice
                        iceFaceTris->push_back(vtl);
                        iceFaceTris->push_back(vbr);
                        iceFaceTris->push_back(Vec3(vbr.x(), vbr.y(), 0.0f));
                        iceFaceTris->push_back(vtl);
                        iceFaceTris->push_back(Vec3(vbr.x(), vbr.y(), 0.0f));
                        iceFaceTris->push_back(Vec3(vtl.x(), vtl.y(), 0.0f));
                        iceFaceTriCount += 2;
                    }
                } else if (tr) {
                    //Forms a right hand triangle
                    iceUpperFaces->push_back(ibl);
                    iceUpperFaces->push_back(ibr);
                    iceUpperFaces->push_back(itr);

                    if (!tl) {
                        //Draw diagonal face of ice
                        iceFaceTris->push_back(vbl);
                        iceFaceTris->push_back(vtr);
                        iceFaceTris->push_back(Vec3(vtr.x(), vtr.y(), 0.0f));
                        iceFaceTris->push_back(vbl);
                        iceFaceTris->push_back(Vec3(vtr.x(), vtr.y(), 0.0f));
                        iceFaceTris->push_back(Vec3(vbl.x(), vbl.y(), 0.0f));
                        iceFaceTriCount += 2;
                    }
                } else {
                    iceFaceTris->push_back(vbl);
                    iceFaceTris->push_back(vbr);
                    iceFaceTris->push_back(Vec3(vbr.x(), vbr.y(), 0.0f));
                    iceFaceTris->push_back(vbl);
                    iceFaceTris->push_back(Vec3(vbr.x(), vbr.y(), 0.0f));
                    iceFaceTris->push_back(Vec3(vbl.x(), vbl.y(), 0.0f));
                    iceFaceTriCount += 2;
                }
            }

            if (tl && tr) {
                if (br) {
                    //Upper right hand triangle
                    iceUpperFaces->push_back(itr);
                    iceUpperFaces->push_back(itl);
                    iceUpperFaces->push_back(ibr);

                    if (!bl) {
                        //Draw diagonal face of ice
                        iceFaceTris->push_back(vbr);
                        iceFaceTris->push_back(vtl);
                        iceFaceTris->push_back(Vec3(vtl.x(), vtl.y(), 0.0f));
                        iceFaceTris->push_back(vbr);
                        iceFaceTris->push_back(Vec3(vtl.x(), vtl.y(), 0.0f));
                        iceFaceTris->push_back(Vec3(vbr.x(), vbr.y(), 0.0f));
                        iceFaceTriCount += 2;
                    }
                } else if (bl) {
                    //Upper left hand triangle
                    iceUpperFaces->push_back(itr);
                    iceUpperFaces->push_back(itl);
                    iceUpperFaces->push_back(ibl);

                    if (!br) {
                        //Draw diagonal face of ice
                        iceFaceTris->push_back(vtr);
                        iceFaceTris->push_back(vbl);
                        iceFaceTris->push_back(Vec3(vbl.x(), vbl.y(), 0.0f));
                        iceFaceTris->push_back(vtr);
                        iceFaceTris->push_back(Vec3(vbl.x(), vbl.y(), 0.0f));
                        iceFaceTris->push_back(Vec3(vtr.x(), vtr.y(), 0.0f));
                        iceFaceTriCount += 2;
                    }
                } else {
                    iceFaceTris->push_back(vtr);
                    iceFaceTris->push_back(vtl);
                    iceFaceTris->push_back(Vec3(vtl.x(), vtl.y(), 0.0f));
                    iceFaceTris->push_back(vtr);
                    iceFaceTris->push_back(Vec3(vtl.x(), vtl.y(), 0.0f));
                    iceFaceTris->push_back(Vec3(vtr.x(), vtr.y(), 0.0f));
                    iceFaceTriCount += 2;
                }
            }

            if (tl && bl && !tr && !br) {
                iceFaceTris->push_back(vtl);
                iceFaceTris->push_back(vbl);
                iceFaceTris->push_back(Vec3(vbl.x(), vbl.y(), 0.0f));
                iceFaceTris->push_back(vtl);
                iceFaceTris->push_back(Vec3(vbl.x(), vbl.y(), 0.0f));
                iceFaceTris->push_back(Vec3(vtl.x(), vtl.y(), 0.0f));
                iceFaceTriCount += 2;
            } else if (tr && br && !tl && !bl) {
                iceFaceTris->push_back(vbr);
                iceFaceTris->push_back(vtr);
                iceFaceTris->push_back(Vec3(vtr.x(), vtr.y(), 0.0f));
                iceFaceTris->push_back(vbr);
                iceFaceTris->push_back(Vec3(vtr.x(), vtr.y(), 0.0f));
                iceFaceTris->push_back(Vec3(vbr.x(), vbr.y(), 0.0f));
                iceFaceTriCount += 2;
            }
        }
    }

    iceUpperSurface->addPrimitiveSet(iceUpperFaces);

    Geometry* iceFaceSurface = new Geometry;
    iceFaceSurface->getOrCreateStateSet()->setAttributeAndModes(iceMaterial.get(), StateAttribute::ON);

    iceFaceSurface->setVertexArray(iceFaceTris);
    iceFaceSurface->addPrimitiveSet(new DrawArrays(PrimitiveSet::TRIANGLES, 0, iceFaceTriCount * 3));

    osgUtil::SmoothingVisitor smoothifier;
    smoothifier.smooth(*waterSurface);
    smoothifier.smooth(*iceFaceSurface);

    geometry->addChild(waterSurface);
    geometry->addChild(iceUpperSurface);
    geometry->addChild(iceFaceSurface);
}

StaticIceTile::StaticIceTile(float t_width, osg::Vec2s coords)
    : TerrainTile(t_width, coords)
{
}

void StaticIceTile::setHeightMap(HeightMap *heightMap)
{
    ref_ptr<Material> iceMaterial = new Material;
    iceMaterial->setColorMode(Material::DIFFUSE);
    iceMaterial->setAmbient(Material::FRONT_AND_BACK, Vec4(0.1, 0.1, 0.1, 1));
    iceMaterial->setDiffuse(Material::FRONT_AND_BACK, Vec4(0.6, 0.6, 0.6, 1.0));
    iceMaterial->setSpecular(Material::FRONT_AND_BACK, Vec4(0.8, 0.8, 0.8, 1));
    iceMaterial->setShininess(Material::FRONT_AND_BACK, 64);

    geomFull = new Geometry;
    geomReduced = new Geometry;
    geomFull->getOrCreateStateSet()->setAttributeAndModes(iceMaterial.get(), StateAttribute::ON);
    geomReduced->getOrCreateStateSet()->setAttributeAndModes(iceMaterial.get(), StateAttribute::ON);

    render(heightMap, geomFull.get());

    HeightMapSimplifier heightSimpl(heightMap, 8);
    render(&heightSimpl, geomReduced.get());
}

void StaticIceTile::updateEyeDist(float dist)
{
    float lowres = 1500.0f;
    float cutoff = 6000.0f;

    if (dist < lowres) {
        setModel(geomFull.get());
    } else if (dist < cutoff) {
        setModel(geomReduced.get());
    } else {
        setModel(nullptr);
    }
}

void StaticIceTile::render(IHeightMap *heightMap, Geometry* geometry)
{
    int vertexCount = heightMap->getWidth() + 1;

    //Load the vertices
    Vec3Array* triangles = new Vec3Array;
    Vec3Array* normals = new Vec3Array;
    for (int x = 0; x < vertexCount; x++) {
        for (int y = 0; y < vertexCount; y++) {
            triangles->push_back(heightMap->getVertex(x, y));
            normals->push_back(heightMap->getNormal(x, y));
        }
    }

    geometry->setVertexArray(triangles);
    geometry->setNormalArray(normals, Array::BIND_PER_VERTEX);

    //Define the faces as elements list
    DrawElementsUInt* faces = new DrawElementsUInt(PrimitiveSet::TRIANGLES, 0);

    for (int x = 0; x < vertexCount-1; x++) {
        for (int y = 0; y < vertexCount-1; y++) {
            uint32_t ibl = x * vertexCount + y;
            uint32_t ibr = (x + 1) * vertexCount + y;
            uint32_t itl = x * vertexCount + (y + 1);
            uint32_t itr = (x + 1) * vertexCount + (y + 1);

            faces->push_back(ibl);
            faces->push_back(ibr);
            faces->push_back(itl);
            faces->push_back(ibr);
            faces->push_back(itr);
            faces->push_back(itl);
        }
    }

    geometry->addPrimitiveSet(faces);
}

/**
 * @brief Construct a terrain tile of given type
 */
TerrainTile* constructTerrainTileType(TerrainTile::TileType type, float t_width, osg::Vec2s coords)
{
    switch (type) {
    case TerrainTile::TYPE_NONE:
        return new TerrainTile(t_width, coords);
    case TerrainTile::TYPE_WATER:
        return new WaterTile(t_width, coords);
    case TerrainTile::TYPE_BORING_ICE:
        return new BoringIceTile(t_width, coords);
    case TerrainTile::TYPE_STATIC_BOUNDARY_ICE:
        return new StaticBoundaryIceTile(t_width, coords);
    case TerrainTile::TYPE_STATIC_ICE:
        return new StaticIceTile(t_width, coords);
    default:
        throw "Not implemented yet";
    }
}
