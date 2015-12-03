# lagrange-interpolation
This class is simple lagrange interpolation fomula making class. 
If the number is added about 30, it is possible to generate an error.

## Useage
``` c++
LagrangeInterpolationInt<uint64_t> lii;

lii.AddValue(1, 1);
lii.AddValue(2, 3);
lii.AddValue(3, 5);
lii.AddValue(4, 7);
lii.AddValue(5, 217341);

std::wcout << lii.GetInterpolation() << std::endl;
std::wcout << lii.GetInterpolation(true) << std::endl;
```
```
18111/2*x*x*x*x+-90555*x*x*x+633885/2*x*x+-452773*x+217331
18111/2x^4-90555x^3+633885/2x^2-452773x^1+217331
```
