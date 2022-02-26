Core: Agent
    |
    Attributes: listed below

Core: Environment
    |
    Attributes: . . .

Agent [is the physical body/bodies we are simulating]
    |
    ID [used to identify the agent]
    |
    Mass [kg] 
    |
    Density
    |
    Velocity
    |
    Acceleration
    |
    Forces
        |
        {Normal force, Gravitational force} 
        |
        Frictional force
    
    |
    Shape [Bounding box]
    |
    Center of mass
    |
    Surface texture/roughness [frictin depends upon this]
    |
    Compressible [how tight the molecules are packed]
    |
    Shape retention [how much of the original shape it is able to retain over time]
    [Strength between the bonds of molecules]
    |
    



Environment [has its own physics properties to interact with the agent]
    |
    Force [direction, magnitude]
        |
        Gravity [constant]
        |
        Point_External [apply force at a point]
    |
    Surface texture/roughness
        |
        Ground
        |
        Walls
    |
    Atmosphere density [drag]
    |
    Spawn [create agents]
        |
        Position [w.r.t in environment frame]

