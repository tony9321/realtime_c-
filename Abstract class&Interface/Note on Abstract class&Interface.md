#Voloridge 
# Abstract Class
class w/ at least one pure virtual func(declared w/ =0)
```cpp
struct Shape {
	virtual void draw()=0; // pure virtual
	virtual ~Shape()=default;
};

struct Circle : Shape {
	void draw() override {std::cout << "Circle\n";}
};
```
`Shape`is abstract here, only its subclass can be instantiated
## Interface
Just an `abstract class`w/ only pure virtual func and no data members
So:
```cpp
struct Drawable {
	virtual void draw()=0;
	virtual ~Drawable()=default;
};
```
Is an interface

### note on python Abstract Base Class(abc) module
abstract base class are like interfaces
```python
class Shape(ABC):
	@abstractmethod
	def draw(self):
		pass
class Circle(Shape):
	def draw(self):
		print("Circle")
Shape() # error, can't instantiate abstract class
Circle.draw() #works
```
## 

| Concept            | C++                                                             | Python                                                                       |
| ------------------ | --------------------------------------------------------------- | ---------------------------------------------------------------------------- |
| **Abstract class** | Class with pure virtual (`=0`) function(s). Cannot instantiate. | Class inheriting from `abc.ABC`, with `@abstractmethod`. Cannot instantiate. |
| **Interface**      | Abstract class with only pure virtuals.                         | Implemented via abstract base classes; no distinct keyword.                  |

