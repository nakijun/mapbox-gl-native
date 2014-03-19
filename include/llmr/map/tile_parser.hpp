#ifndef LLMR_MAP_TILE_PARSER
#define LLMR_MAP_TILE_PARSER

#include <llmr/map/tile_data.hpp>
#include <llmr/text/placement.hpp>

namespace llmr {

class TileParser {
public:
    TileParser(const std::string& data, TileData& tile, const Style& style, GlyphAtlas& glyphAtlas);

private:
    bool obsolete() const;
    void parseGlyphs();
    void parseStyleLayers(const std::vector<LayerDescription>& layers);
    std::shared_ptr<Bucket> createBucket(const BucketDescription& bucket_desc);
    std::shared_ptr<Bucket> createFillBucket(const VectorTileLayer& layer, const BucketDescription& bucket_desc);
    std::shared_ptr<Bucket> createLineBucket(const VectorTileLayer& layer, const BucketDescription& bucket_desc);
    std::shared_ptr<Bucket> createPointBucket(const VectorTileLayer& layer, const BucketDescription& bucket_desc);
    std::shared_ptr<Bucket> createTextBucket(const VectorTileLayer& layer, const BucketDescription& bucket_desc);
    template <class Bucket> void addBucketFeatures(Bucket& bucket, const VectorTileLayer& layer, const BucketDescription& bucket_desc);

private:
    const VectorTile data;
    TileData& tile;
    const Style& style;
    GlyphAtlas& glyphAtlas;
    FaceGlyphPositions faces;
    Placement placement;
};

}

#endif
