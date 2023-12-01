COMP3811 Exercise G.5
=====================

Code for Exercise G.5 in COMP3811. Do not re-distribute the code outside of
Minerva.

## The .comp3811bin file structure

This is a simple, custom binary file format. It has 6 sections. In order:

1. The file "magic". This is a (hopefully) unique identifier, used to
   discriminate valid .comp3811bin meshes from other files. It is always
   exactly 16 bytes and must be equal to the C/C++ string 

   "\0COMP3811mesh00"

2. Vertex and index counts stored as two consecutive 32-bit unsigned integers.

For the remaining, assume that V is the vertex count and I is the index count
from Step 2.

3. Indices. This consists of I 32-bit unsigned integer indices.

4. Vertex positions. This consists of V tuples of three floats. Positions are
   stored as { x, y, z }.

5. Vertex colors. This consists of V tuples of three floats. Colors are stored
   as { r, g, b }.

6. Vertex normals. This consists of V tuples of three floats. Normals are
   stored as { x, y, z }.

(The file should end here; any additional data is ignored.)
