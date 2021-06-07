unsigned int htoi(const char str[])
{
    /* Local Variables */
    unsigned int result;
    int i;

    i = 0;
    result = 0;
    
    if(str[i] == '0' && str[i+1] == 'x')
    {
        i = i + 2;
    }

    while(str[i] != '\0')
    {
        result = result * 16;
        if(str[i] >= '0' && str[i] <= '9')
        {
            result = result + (str[i] - '0');
        }
        else if(tolower(str[i]) >= 'a' && tolower(str[i]) <= 'f')
        {
            result = result + (tolower(str[i]) - 'a') + 10;
        }
        i++;
    }

    return result;
}

/* getBinary
 *
 * Converts an unsigned integer into a string containing it's
 * 32 bit long binary representation.
 *
 *
 * @param   num         number to be converted
 *
 * @result  char*       binary string
 */
 
char *getBinary(unsigned int num)
{
    char* bstring;
    int i;
    
    /* Calculate the Binary String */
    
    bstring = (char*) malloc(sizeof(char) * 33);
    assert(bstring != NULL);
    
    bstring[32] = '\0';
    
    for( i = 0; i < 32; i++ )
    {
        bstring[32 - 1 - i] = (num == ((1 << i) | num)) ? '1' : '0';
    }
    
    return bstring;
}

/* formatBinary
 *
 * Converts a 32 bit long binary string to a formatted version
 * for easier parsing. The format is determined by the TAG, INDEX,
 * and OFFSET variables.
 *
 * Ex. Format:
 *  -----------------------------------------------------
 * | Tag: 18 bits | Index: 12 bits | Byte Select: 4 bits |
 *  -----------------------------------------------------
 *
 * Ex. Result:
 * 000000000010001110 101111011111 00
 *
 * @param   bstring     binary string to be converted
 *
 * @result  char*       formated binary string
 */

char *formatBinary(char *bstring)
{
    char *formatted;
    int i;
    
    /* Format for Output */
    
    formatted = (char *) malloc(sizeof(char) * 35);
    assert(formatted != NULL);
    
    formatted[34] = '\0';
    
    for(i = 0; i < TAG; i++)
    {
        formatted[i] = bstring[i];
    }
    
    formatted[TAG] = ' ';
    
    for(i = TAG + 1; i < INDEX + TAG + 1; i++)
    {
        formatted[i] = bstring[i - 1];
    }
    
    formatted[INDEX + TAG + 1] = ' ';
    
    for(i = INDEX + TAG + 2; i < OFFSET + INDEX + TAG + 2; i++)
    {
        formatted[i] = bstring[i - 2];
    }

    return formatted;
}

/* btoi
 *
 * Converts a binary string to an integer. Returns 0 on error.
 *
 * src: http://www.daniweb.com/software-development/c/code/216372
 *
 * @param   bin     binary string to convert
 *
 * @result  int     decimal representation of binary string
 */

int btoi(char *bin)
{
    int  b, k, m, n;
    int  len, sum;

    sum = 0;
    len = strlen(bin) - 1;

    for(k = 0; k <= len; k++)
    {
        n = (bin[k] - '0'); 
        if ((n > 1) || (n < 0))
        {
            return 0;
        }
        for(b = 1, m = len; m > k; m--)
        {
            b *= 2;
        }
        sum = sum + n * b;
    }
    return(sum);
}

/* parseMemoryAddress
 *
 * Helper function that takes in a hexidecimal address in
 * the format of "0x00000000" and spits out the decimal, 
 * binary, and formatted binary equivilants. Also, it 
 * calculates the corresponding tag, index, and offset.
 *
 * @param       address         Hexidecimal memory address
 *
 * @return      void
 */

void parseMemoryAddress(char *address)
{
    unsigned int dec;
    char *bstring, *bformatted, *tag, *index, *offset;
    int i;
    
    dec = htoi(address);
    bstring = getBinary(dec);
    bformatted = formatBinary(bstring);
    
    if(DEBUG)
    {
        printf("Hex: %s\n", address);
        printf("Decimal: %u\n", dec);
        printf("Binary: %s\n", bstring);
        printf("Formatted: %s\n", bformatted);
    }
    
    i = 0;
    
    tag = (char *) malloc( sizeof(char) * (TAG + 1) );
    assert(tag != NULL);
    tag[TAG] = '\0';
    
    for(i = 0; i < TAG; i++)
    {
        tag[i] = bformatted[i];
    }
    
    index = (char *) malloc( sizeof(char) * (INDEX + 1) );
    assert(index != NULL);
    index[INDEX] = '\0';
    
    for(i = TAG + 1; i < INDEX + TAG + 1; i++)
    {
        index[i - TAG - 1] = bformatted[i];
    }
    
    offset = (char *) malloc( sizeof(char) * (OFFSET + 1) );
    assert(offset != NULL);
    offset[OFFSET] = '\0';
    
    for(i = INDEX + TAG + 2; i < OFFSET + INDEX + TAG + 2; i++)
    {
        offset[i - INDEX - TAG - 2] = bformatted[i];
    }
    
    printf("Tag: %s (%i)\n", tag, btoi(tag));
    printf("Index: %s (%i)\n", index, btoi(index));
    printf("Offset: %s (%i)\n", offset, btoi(offset));
}
