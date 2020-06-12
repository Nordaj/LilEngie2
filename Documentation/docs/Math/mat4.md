# Lil Math - mat4
`#include <Core/Math/LilMath.h>`

The 4x4 Matrix class for Lil Math
<br /><br />

mat4 uses a column major matrix order

---

## Constructors
```cpp
mat4()
```
Empty constructor initializes matrix to identity matrix
<br /><br />

```cpp
mat4(float d)
```
Initializes a matrix with the given diagonal d
<br /><br />

```cpp
mat4(const float* e)
```
Initializes matrix using float array, assumes 16 floats in size
<br /><br />

```cpp
mat4(const vec4* col)
```
Initializes based on array of column vectors, assumes 4 vec4s in size
<br /><br />

```cpp
mat4(vec4 r0, vec4 r1, vec4 r2, vec4 r3)
```
Constructs matrix using row vectors for easy to read construction
<br /><br />

---

## Methods
```cpp
vec4 row(int i) const
```
Returnes the row vector of given index i
<br /><br />

---

## Functions
```cpp
vec4 mul(const mat4& m, const vec4& v)
```
Multiplies mat4 by vec4
<br /><br />

```cpp
mat4 mul(const mat4& a, const mat4& b)
```
Multiplies the two matrices
<br /><br />

```cpp
mat4 translate(vec3 v)
```
Generates a translation matrix given a single translation vector v. The translation goes on the 4th column on first 3 rows in xyz order.
<br /><br />

```cpp
void translate(mat4& m, vec3 v)
```
Translates a given matrix by the vector v.
<br /><br />

```cpp
mat4 rotate(vec3 euler)
```
Generates a rotation matrix given a 3d vector representing euler angles. The rotation order is XYZ. And each axis is (Pitch, Yaw, Roll) to rotate about their own axis.
<br /><br />

```cpp
void rotate(mat4& m, vec3 euler)
```
Rotates a given matrix the euler angles.
<br /><br />

```cpp
mat4 scale(vec3 v)
```
Generates a scale matrix
<br /><br />

```cpp
void scale(mat4& m, vec3 v)
```
Scales the given matrix by v
<br /><br />

```cpp
mat4 projection(float r, float l, float t, float b, float n, float f)
```
Creates a perspective projection matrix with components: 
right, left, top, bottom, near, far.
The larger the difference in z, the less precision in a given unit for the depth buffer. Also keep in mind that the depth buffer is more precise closer to the camera as this will place things non-linearly depth wise. 
<br /><br />

```cpp
float determinant(const mat4& m)
```
Calculates the determinant of the given matrix
<br /><br />

```cpp
mat4 inverse(const mat4& m)
```
Calculates the inverse of the given matrix, if the determinant is 0 it will return a matrix with all elements of 0.
<br /><br />

```cpp
mat4 transpose(const mat4& m)
```
Transposes the given matrix and returns it as a new one
<br /><br />

---

## Union
|||
|--|--|
`float m[16]`|Each individual element of the matrix in column major order as a 4 byte float
`vec4 v[4]`|Each individual column vector of the matrix