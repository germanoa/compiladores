
int : fat (int : x)
{
  if (x == 1) then output 1
  output x*fat(x-1)
}

int : main()
    int: var1
{
    //commentario
    var1 = 1 + 3
    output fat(var1)
}
