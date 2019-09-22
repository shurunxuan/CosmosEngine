//
// Created by shuru on 9/7/2019.
//

#include "RenderingPipeline.h"


RenderingPipeline::RenderingPipeline(Mesh* mesh, Material* material)
{
    this->mesh = mesh;
    this->material = material;
}

RenderingPipeline::~RenderingPipeline()
= default;
