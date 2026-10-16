#include "WanderersDescentGameMode.h"
#include "Characters/WanderersDescentCharacter.h"

AWanderersDescentGameMode::AWanderersDescentGameMode()
{
	DefaultPawnClass = AWanderersDescentCharacter::StaticClass();
}
