# Output Using home_day.jpg and home_night.jpg
----

## Translation
```
$ ./register.exe images/home_day.jpg images/home_night.jpg -m=translation

Shortcuts:
        q       - quit

Image size is:                  1296x782
Image size is:                  1296x782

Original Warp Matrix:
[1,     0,      0]
[0,     1,      0]

Transformed Warp Matrix:
[1,     0,      0.884631]
[0,     1,      2.41713]

Motion Type: translation

Correlation Coefficient: 0.269999
Wrote ./out/translation_out_warp.ecc
Wrote ./out/translation_warped_ecc.jpg
```
----

## Euclidean
```
$ ./register.exe images/home_day.jpg images/home_night.jpg -m=euclidean
Shortcuts:
        q       - quit

Image size is:                  1296x782
Image size is:                  1296x782

Original Warp Matrix:
[1,     0,      0]
[0,     1,      0]

Transformed Warp Matrix:
[0.999996,      -0.00295896,    1.93121]
[0.00295896,    0.999996,       0.682989]

Motion Type: euclidean

Correlation Coefficient: 0.270456
Wrote ./out/euclidean_out_warp.ecc
Wrote ./out/euclidean_warped_ecc.jpg
```
----

## Affine
```
$ ./register.exe images/home_day.jpg images/home_night.jpg -m=affine

Shortcuts:
        q       - quit

Image size is:                  1296x782
Image size is:                  1296x782

Original Warp Matrix:
[1,     0,      0]
[0,     1,      0]

Transformed Warp Matrix:
[1.00353,       0.00494333,     -2.87226]
[0.00459992,    0.999873,       -0.200684]

Motion Type: affine

Correlation Coefficient: 0.273676
Wrote ./out/affine_out_warp.ecc
Wrote ./out/affine_warped_ecc.jpg
```
----

## Homography
```
$ ./register.exe images/home_day.jpg images/home_night.jpg -m=homography

Shortcuts:
        q       - quit

Image size is:                  1296x782
Image size is:                  1296x782

Original Warp Matrix:
[1,     0,      0]
[0,     1,      0]
[0,     0,      1]

Transformed Warp Matrix:
[0.972042,      -0.0187662,     8.2457]
[-0.000339586,  0.958759,       7.64147]
[-1.60638e-05,  -4.13832e-05,   1]

Motion Type: homography

Correlation Coefficient: 0.278261
Wrote ./out/homography_out_warp.ecc
Wrote ./out/homography_warped_ecc.jpg
```
----
