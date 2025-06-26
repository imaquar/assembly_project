# Root finding and definite integral calculation

## Description
This project calculates the area of a shape bounded by three curves with a given precision.

The implementation also includes:
- Finding points of intersection using the chord (secant) method
- Calculating definite integrals using the rectangle method

## Curve Functions
1. f1(x) = 3/((x-1)^2 + 1)
2. f2(x) = sqrt(x + 0.5)
3. f3(x) = e^(-x)

## Building the Project
The project is built using the `make` utility:

- **Build the entire projec:**
  ```sh
  make
  ```

- **Run tests:**
  ```sh
  make test
  ```
  Example test: `./integral --test-root 1:2:1.8:2.2:0.0001:1.95615`
  
  Output format: `вычисленный_результат абсолютная_ошибка относительная_ошибка`
  
- **Clean the project:**
  ```sh
  make clean
  ```

## Program Usage
- `./integral` Compute the area of the shape.
- `./integral -h` или `./integral --help` Show usage help.
- `./integral -r` или `./integral --root` Display the points of intersection of the curves.
- `./integral -i` или `./integral --iterations` Show the number of iterations for root finding.
- `./integral -R ...` или `./integral --test-root ...` Test the root-finding function.

  Parameter format (`...`): `F1:F2:A:B:E:R`
  
  Example: `./integral -R 1:2:1.8:2.2:0.0001:1.95615`
- `./integral -I ...` или `./integral --test-integral ...` Test the integral calculation function.
  
  Parameter format (`...`): `F:A:B:E:R`
  
  Example: `./integral -I 1:-0.20333:1.95615:0.0001:4.92110`