**2D Rigid Body Dynamics**\
The “rigid body” part of rigid body dynamics refers to constraints we place upon the objects we’re simulating. A rigid body’s shape does not change during our simulation—it’s more like wood or metal than jello. We can still create articulated figures, such as a human being, by building each section of the figure from a rigid body and putting joints between them, but we won’t account for the flexing of bones under strain or similar effects. This will let us simplify our equations while still allowing for interesting dynamic behavior.

**Dynamics Algorithm**
1. Calculate the CM and the moment of inertia at the CM.
2. Set the body’s initial position, orientation, and linear and angular velocities.
3. Figure out all of the forces on the body, including their points of application.
4. Sum all the forces and divide by the total mass to find the CM’s linear acceleration.
5. For each force, form the perpdot product from the CM to the point of force application and add the value into the total torque at the CM
6. Divide the total torque by the moment of inertia at the CM to find the angular acceleration 
7. Numerically integrate the linear acceleration and angular acceleration to update the position, linear velocity, orientation, and angular velocity.
8. Draw the object in the new position, and go to Step 3

**Reference**
1. http://www.chrishecker.com/Rigid_Body_Dynamics#Physics_Articles
