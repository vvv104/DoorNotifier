#include "Debug.h"
#include "Door.h"

static Door door;

void setup()
{
  door.Start();
}

void loop()
{
  door.Step();
}
