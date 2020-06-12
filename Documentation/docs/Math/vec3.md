# Lil Math - vec3
`#include <Core/Math/LilMath.h>`

The 3D vector class for Lil Math

---

## Constructors
```cpp
vec4()
```
Empty constructor initializes all components to 0
<br /><br />

```cpp
vec4(float x, float y, float z)
```
Initializes corresponding elements in XYZ order
<br /><br />

---

## Functions
```cpp
float dot(vec3 a, vec3 b)
```
Returns scalar result of dot product. (Sum of product of each component)
<br /><br />

```cpp
foat len(vec3 vec)
```
Calculates the real length of the given vector
<br /><br />

```cpp
float len2(vec3 vec)
```
Calculates the squared length of the given vector. This is a much faster operation. Is the equivilent of dot(v, v)
<br /><br />

```cpp
void normalize(vec3& vec)
```
Normalizes given vector to make vector length of 1
<br /><br />

```cpp
vec3 cross(vec3 a, vec3 b)
```
Calculates the cross product of `a x b`
<br /><br />

---

## Union
|||
|--|--|
`x, y, z`|Each individual component as 4 byte floats
`r, g, b`|Each individual component using color variables as 4 byte floats
`s[3]`|Each component stored as a 4 element array