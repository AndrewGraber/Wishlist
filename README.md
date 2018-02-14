# Wishlist
A simple but useful program that generates a priority based wishlist ranked by different attributes for each item.

*Usage*: upon compilation, requires two parameters:
  1. The name of the file that has the values required
  2. The name for the output file
  
*Input File Format:*
The file should consist of however many entries you want to put in, one per line.
For each entry, there should be a name (anything you want, less than or equal to 20 characters) followed by a colon.
After the colon, there should be 3 floating point values. These are: "desire", "usefulness", "urgency" respectively.
  - Desire: A positive number based on how strong your desire for the item is
  - Usefulness: A positive number based on how useful the item will be
  - Urgency: How urgently the item is needed
  
Also, after the last item, there should be an empty line. I will get around to fixing this eventually

*Example Input File:*
```
Cat Food: 4 9 10
A New Car: 10 8.5 1
That new game: 10 2.5 4.436

```

The weights of each of these values can be adjusted by changing the preprocessor definitions at the top of wishlist.cpp

And yeah, I did use C++ solely for the `bool`s. I know it's terrible

Let me know if you find any problems.
