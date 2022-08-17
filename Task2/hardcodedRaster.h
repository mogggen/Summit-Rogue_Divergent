
namespace Game
{
#define ooo 0, 0, 0,
#define ooi 0, 0, 1,
#define oio 0, 1, 0,
#define oii 0, 1, 1,
#define ioo 1, 0, 0,
#define ioi 1, 0, 1,
#define iio 1, 1, 0,
#define iii 1, 1, 1,

    const bool* CharacterRasterization(/* bool& coordinatesToRender */ const char c)
    {
    //    XXX
    //    XXX
    //    XXX
    //    XXX
    //    XXX

        bool* coordinatesToRender = new bool[15];

        for (size_t i = 0; i < sizeof(coordinatesToRender); i++)
        {
            coordinatesToRender[i] = 0x0;
        }

        switch (c)
        {
        case ' ':
            coordinatesToRender = new bool[15]
            {
                ooo
                ooo
                ooo
                ooo
                ooo
            };
            break;

        case '0':
            coordinatesToRender = new bool[15]
            {
                iii
                ioi
                ioi
                ioi
                iii
            };
            break;
        case '1':
            coordinatesToRender = new bool[15]
            {
                oio
                iio
                oio
                iii
            };
        case '2':
            coordinatesToRender = new bool[15]
            {
                iii
                ooi
                iii
                ioo
                iii
            };
        case '3':
            coordinatesToRender = new bool[15]
            {
                iii
                ooi
                iii
                ooi
                iii
            };
            break;
        }
        return coordinatesToRender;
    }

}
