
## Todo's for the raster library:

- Implement the check bounds routine (check for partial AND full out of bounds).

- Implement the clipping of sprites etc when part of the sprite is off screen.

- Implement routine 'plot_clipped_bitmap() to plot ' to plot a bitmap that is partially off screen.

```
plot_clipped_bitmap_flow{

    if (partal_offscreen) --> clip the bitmap --> 'plot_clipped_bitmap()' routine

    else if (full_offscreen) --> skip the drawing routine

}
```

- Refactor the raster library (including the .h file) to use INT instead of UINT, because negative values need to be possible (when objects go off screen).

