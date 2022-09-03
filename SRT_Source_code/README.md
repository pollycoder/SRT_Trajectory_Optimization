# SRT_Trajectory_Optimization_Source_Code

## This is a repository containing all the tool code probably going to be used in the project.

## Commonly used tools: ( Detailed tutorial is included in the file )

### Lambert (.cpp + .hpp)
The function of Lambert Module is to solve two-body problem.
Suitable scene: One central gravity source + one moving areo

### PSO_DE_NLOPT (.cpp + .hpp)
The function of PSO_DE_NLOPT Module is to do optimization and to solve the semi-best solution.
Normally use PSO is enough.

### Ephemeris (.cpp + .h)
The function of Ephemeris Module is to import data from the Ephemeris (JPL,from NASA's blog).
Pay attention to the scale and unit system.

### OrbitFun & OrbitMath (.cpp +.h)
These two modules are always used together.
Their function is to do the math work to figure out the six Orbital Roots.
