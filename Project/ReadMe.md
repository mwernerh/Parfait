# Controls
* Press A & D to move
* Press K to attack

# Information
* Level changes at a score of 1,000
    * Causes enemies to despawn on level change
* Blue Dogs
    * Slow, but more health
* Yellow Dogs
    * Faster, but only one health

# Known Issues
* Enemies cannot yet attack
    * They do have attacking hitboxes, though, so it's just a matter implementing an Attack() function for the enemy, and adding a for loop in the HitboxManager update function