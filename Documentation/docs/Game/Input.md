# Input
`#include <Core/Game/Input.h>`

The input management class for LilEngie

---
## Usage
The Input class is to be created and maintained by the core Game class. You should only either access it through the game class or through the static core pointer. 

An example use of the input class could look something like:
```cpp
if (input->GetKeyDown(Key::Space))
	//Jump
```

---

## Properties
```cpp
bool lockMousePos;
```
When true, Input will move the cursor to the centor of the screen each frame. It will continue to get mouse delta information. This can be set at any time and will act accordingly in the following frames. If you are to lock the mouse, you will probably also want to use MouseVisibility(true) to set the cursor to invisible. Keep in mind you must also provide an escape from a locked/invisible cursor.
<br /><br />

```cpp
bool mouseVisible;
```
Setting this property will do nothing, it is only meant to be read to mouse visibility toggle behaviour. If you want to set the mouse visibility you must use MouseVisibility(bool) which will then update mouseVisible by itself.
<br /><br />

Input has other public properties such as mouse states and key states that are avaliable to read input directly but not recommended. Everything you need should be accomplishable from not accessing these. But just in case you would prefer to access this data yourself, they are avaliable.

---

## Methods
```cpp
bool GetKey(Key key)
bool GetKeyDown(Key key)
bool GetKeyUp(Key key)
```
The get key methods allow easy access to key states. They are updated once per frame so you can trust that you will not miss a key input. `GetKey(Key)` will return the current key state. Whereas the Down and Up methods will only be true for a single frame at a time which can be used for press events. For example, you could use `GetKeyDown(Key::Space)` for jumping behaviour which you do not want to persist for multiple frames, rather only the first frame after the player presses space.
<br /><br />

```cpp
bool GetMouseBtn(MouseButton button)
bool GetMouseBtnDown(MouseButton button)
bool GetMouseBtnUp(MouseButton button)
```
The mouse button methods are similar to the GetKey methods except... they are for the mouse. MouseButton supports 5 button inputs for the mouse. That is, left, right, scroll button, and the side navigation buttons (XButton1, XButton2).
<br /><br />

```cpp
vec3 GetMousePos()
```
Returns the cursor position in pixels as a vec3 from the top left of the screen. The z component is always 0 and it will never be more precise than whole numbers. I only use vec3 here because I have not yet implemented a vec2...
<br /><br />

```cpp
vec3 GetMouseDelta()
```
Returns the raw mouse movement sent directly from the device. (The numbers get pretty large, in the hundreds usually).
<br /><br />

```cpp
float GetWheelDelta()
```
Gets the scroll wheel delta. Positive is forward (up), negative is backwards(down). Is usually in units of 120 for some reason (ask Win API why). 
<br /><br />

```cpp
void MouseVisibility(bool visible)
```
Sets the visibility state (or hides/shows) for the mouse. Will also set the `mouseVisible` bool for you to read from to check the current state.
<br /><br />

---
