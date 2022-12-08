// Change to any day to run that day's code
#define DAY Day1

// Gets around include nonsense, just let compiler figure it out
namespace DAY
{
	extern int main();
}

int main()
{
	return DAY::main();
}
