#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

class Foo
{
  public:
    void foo()
    {

    }
};

class Bar : public Foo
{
  public:
    void bar()
    {

    }
};

int main(void)
{
  SystemInit();
  SystemCoreClockUpdate();
  Foo foo;
  Bar bar;
  foo.foo();
  bar.foo();
  bar.bar();
  while (1)
  {
  }
}
