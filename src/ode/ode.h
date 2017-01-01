#ifndef ODE_H
#define ODE_H

// no members initialization in c++03 :(

class ODE {
public:
  ODE();
  virtual ~ODE(){};
  void RK4(double *);
  double t;
  void set_dt(double);
  double getStep() { return dt; }

protected:
  double dt;
  virtual double *dx(double *);

private:
};

#endif // ODE_H
