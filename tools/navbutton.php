<? // http://www.thescripts.com/serversidescripting/php/articles/dynamicimagesinphp3.0/page0.html
// Prepare the text - remove unwanted characters etc
$string = implode($argv," ");
$string = stripslashes($string);
$string = ereg_replace("\r\n", "\n", $string) ;
$string = ereg_replace("%20",  " ", $string);

// Create the image
$myimage = ImageCreateFromGif("images/unselected.gif");
if(!$myimage) {
    // No image, create a blank one
    $myimage = ImageCreate(255, 19);
    $white   = ImageColorAllocate($myimage, 255, 255, 255);
    ImageFilledRectangle($myimage, 0, 0, 255, 19, $white);
}

// Setup colors
$black = ImageColorAllocate($myimage, 0, 0, 0);
$width = (ImageSX($myimage)-7.5*strlen($string))/2;
    
// Write text to image
ImageString($myimage, 3, $width, 3, $string, $black);
    
// Show image
Header("Content-Type: image/gif");
ImageGif($myimage);
ImageDestroy($myimage);
?>


