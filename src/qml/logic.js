function getTypeFromChar(type) 
{
    switch (type) {
    case '-': return "";
    case '1': return "PlainBrick1";
    case '2': return "PlainBrick2";
    case '3': return "PlainBrick3";
    case '4': return "PlainBrick4";
    case '5': return "PlainBrick5";
    case '6': return "PlainBrick6";
    case 'm': return "MultipleBrick3";
    case 'x': return "ExplodingBrick";
    case 'u': return "UnbreakableBrick";
    case 'h': return "HiddenBrick";
    default:
        print("Invalid File: unknown character '", type, "'");
        return "PlainBrick1";
    }
}

