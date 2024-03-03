#include "CriticalPathCalculator.h"
#include "DataTypes.h"

#include <iostream>

namespace CriticalPath
{
    CriticalPathCalculator::CriticalPathCalculator(DataManager* data_manager)
        : data_manager(data_manager) {};

    void CriticalPathCalculator::run()
    {
        outputCriticalPath();
    }

    void CriticalPathCalculator::outputCriticalPath()
    {
        double criticalPath = computeCriticalPath();
        cout << "Critical Path(ns) = " << criticalPath << endl;
    }
    double CriticalPathCalculator::computeCriticalPath()
    {
        double criticalPath = 0.0;
        for (wire*& currWire : data_manager->wires)
        {
            if (currWire->type == WireType::INPUT)
            {
                double pathDelay = computeCriticalPath(currWire);
                if (pathDelay > criticalPath)
                {
                    criticalPath = pathDelay;
                }
            }
        }
        for (component*& currComponent : data_manager->components)
        {
            if (currComponent->type == ComponentType::REG)
            {
                double pathDelay = getComponentDelay(currComponent);
                pathDelay += computeCriticalPath(currComponent->outputs["q"]->connection);
                if (pathDelay > criticalPath)
                {
                    criticalPath = pathDelay;
                }
            }
        }
        return criticalPath;
    }

    double CriticalPathCalculator::computeCriticalPath(wire* currWire)
    {
        double criticalPath = 0.0;
        if (currWire->type == WireType::OUTPUT) {
            return 0.0;
        }
        for (port*& currPort : currWire->dest)
        {
            component* currComponent = currPort->parent;
            if (currComponent->type != ComponentType::REG){
                double componentDelay = getComponentDelay(currComponent);
                double maxPathDelay = 0.0;
                if (currComponent->type != ComponentType::REG)
                {
                    auto begin = currComponent->outputs.begin();
                    auto end = currComponent->outputs.end();
                    for (auto it = begin; it != end; ++it)
                    {
                        wire* nextWire = it->second->connection;
                        double pathDelay = computeCriticalPath(nextWire);
                        if (pathDelay > maxPathDelay)
                        {
                            maxPathDelay = pathDelay;
                        }
                    }
                }
                maxPathDelay += componentDelay;
                if (maxPathDelay > criticalPath)
                {
                    criticalPath = maxPathDelay;
                }
            }
        }
        return criticalPath;
    }

    double CriticalPathCalculator::getComponentDelay(component* currComponent)
    {
        int componentIdx = ComponentTypeToInt(currComponent->type);
        int widthIdx = -1;
        switch (currComponent->width)
        {
            case 1:
                widthIdx = 0;
                break;
            case 2:
                widthIdx = 1;
                break;
            case 8:
                widthIdx = 2;
                break;
            case 16:
                widthIdx = 3;
                break;
            case 32:
                widthIdx = 4;
                break;
            case 64:
                widthIdx = 5;
                break;
        }
        return COMPONENT_DELAY[componentIdx][widthIdx];
    }
}