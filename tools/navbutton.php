<? // http://www.thescripts.com/serversidescripting/php/articles/dynamicimagesinphp3.0/page0.html
include("./include/pql.inc");
include("./include/pql_bind9.inc");

$string = implode($argv," ");

// Prepare the text - remove unwanted characters etc
$string = stripslashes($string);
$string = ereg_replace("\r\n", "\n", $string) ;
$string = ereg_replace("%20",  " ", $string);

// Create the image
if(function_exists('ImageCreateFromPng') && (imagetypes() & IMG_PNG)) {
    $myimage = ImageCreateFromPng("images/unselected.png");

    if($myimage)
      $imgtype = 'png';
} elseif(function_exists('ImageCreateFromGif') && (imagetypes() & IMG_GIF)) {
    $myimage = ImageCreateFromGif("images/unselected.gif");

    if($myimage)
      $imgtype = 'gif';
} elseif(function_exists('ImageCreateFromJpeg') && (imagetypes() & IMG_JPG)) {
    $myimage = ImageCreateFromJpeg("images/unselected.jpeg");

    if($myimage)
      $imgtype = 'jpeg';
}

if(!$myimage) {
    // No image, create a blank one
    $myimage = ImageCreate(255, 19);
    $white   = ImageColorAllocate($myimage, 255, 255, 255);
    ImageFilledRectangle($myimage, 0, 0, 255, 19, $white);
}

// Setup colors
$black = ImageColorAllocate($myimage, 0, 0, 0);
    
// Convert the string
$string = pql_maybe_idna_decode($string);

// Write text to image (after decoding it to fix international characters)
imageTTFText($myimage, 10, 0, 10, 14, $black, realpath("include/thryn.ttf"), $string);
    
// Show image
if($imgtype == 'png') {
    Header("Content-Type: image/png");
    ImagePng($myimage);
} elseif($imgtype == 'gif') {
    Header("Content-Type: image/gif");
    ImageGif($myimage);
} elseif($imgtype == 'jpeg') {
    Header("Content-Type: image/jpeg");
    ImageJpeg($myimage);
}

ImageDestroy($myimage);
?>


