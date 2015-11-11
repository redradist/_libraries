#include "iostream"
#include "../../patterns/observer/event.h"
#include "../../patterns/observer/delegate.h"

using namespace evt;
using namespace dlgt;

struct TShapes
{
   virtual void Square(int i)
   {
      std::cout << "TShapes::Square:          " << i << std::endl;
   }
   void Triangle(int i)
   {
      std::cout << "TShapes::Triangle:        " << i << std::endl;
   }
   int FreeG(int i, double k)
   {
      std::cout << "TShapes::FreeG:                     " << i << " " << k << std::endl;
      return 1;
   }
};

struct TDerivedShapes : TShapes
{
   virtual void Square(int i)
   {
      std::cout << "TDerivedShapes::Square:   " << i << std::endl;
   }
};

struct TThings
{
   void Thing1(int i)
   {
      std::cout << "TThings::Thing1:          " << i << std::endl;
   }
   static void Thing2(int i)
   {
      std::cout << "TThings::Thing2:          " << i << std::endl;
   }
   int FreeG(int i, double k)
   {
      std::cout << "TThings::FreeG:                     " << i << " " << k << std::endl;
      return 1;
   }
};

int Free(int i, double k)
{
   std::cout << "Free:                     " << i << std::endl;
   return 1;
}

int FreeG(int i, double k)
{
   std::cout << "FreeG:                     " << i << " " << k << std::endl;
   return 1;
}

int main()
{
   // As usual, a typedef makes life easier when using templates.
   using MyEvent = evt::Event<int(*)(int, double)>;

   TShapes        shapes;
   TDerivedShapes shapes2;
   TThings        things;

   MyEvent event;
   auto delegate = Delegate<int(*)(int, double)>(shapes, &TShapes::FreeG);
   delegate(1,2);
   //makedDeletate(2,1);
   MyEvent::CallBack calll = MyEvent::CallBack(&FreeG);
   auto caller = make_callback(&FreeG);
   event += &caller;
   MyEvent::Listener listener = MyEvent::Listener(&shapes, &TShapes::FreeG);
   add_listener(event, shapes, &TShapes::FreeG);
   event += new MyEvent::Listener(&shapes, &TShapes::FreeG);
   auto delag = Delegate<int(*)(int, double)>(shapes, &TShapes::FreeG);
   event += &delag;
   auto g = make_event(&FreeG);

   /// Invoke the multicast event 
   std::cout << "<multicast>" << std::endl;
   event(100,2);
   std::cout << "</multicast>" << std::endl;

   return 0;
}