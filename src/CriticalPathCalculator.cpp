#include "CriticalPathCalculator.h"
#include "DataTypes.h"

#include <algorithm>
#include <iostream>

namespace DataPathGen
{
    // Class constructor provides access to shared data manager
    CriticalPathCalculator::CriticalPathCalculator(DataManager* data_manager)
        : data_manager(data_manager) {};

    // Function runs critical path calculation
    void CriticalPathCalculator::run()
    {
        outputCriticalPath();
    }

    // Function outputs the critical path
    void CriticalPathCalculator::outputCriticalPath()
    {
        // Compute critical path
        double criticalPath = computeCriticalPath();

        // Output critical path
        cout << "Critical Path : " << criticalPath << " ns" << endl;
    }

    // Function computs the critical path
    double CriticalPathCalculator::computeCriticalPath()
    {

        // Initialize critical path
        double criticalPath = 0.0;

        // Loop through all the wires in the data manager
        for (wire*& currWire : data_manager->wires)
        {

            // Consider all input wires except clock and reset
            if ((currWire->type == WireType::INPUT) && ((currWire->name != "Clk") && (currWire->name != "Rst")))
            {
                // Compute maximum path delay starting at input
                double pathDelay = computeCriticalPath(currWire);

                // Update critical path
                criticalPath = max(pathDelay, criticalPath);
            }
        }

         // Loop through all the components in the data manager
        for (component*& currComponent : data_manager->components)
        {
            // Consider only register components
            if (currComponent->type == ComponentType::REG)
            {
                // Get maximum path delay starting at register
                // Delay of register component included at beginning of paths only
                double pathDelay = getComponentDelay(currComponent);
                pathDelay += computeCriticalPath(currComponent->outputs["q"]->connection);
                
                // Update critical path
                criticalPath = max(pathDelay, criticalPath);
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
} // namespace DataPathGen