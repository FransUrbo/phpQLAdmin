<? // http://www.thescripts.com/serversidescripting/php/articles/dynamicimagesinphp3.0/page0.html
$string=implode($argv," ");
$string = stripslashes($string);
$string = ereg_replace("\r\n","\n",$string) ;
$string = ereg_replace("%20"," ",$string) ;

$myimage = ImageCreateFromGif("images/unselected.gif");
$black   = ImageColorAllocate($myimage, 0, 0, 0);
$width   = (imagesx($myimage)-7.5*strlen($string))/2;

ImageString($myimage, 3, $width, 1, $string, $black);

# Show image
Header("Content-Type: image/gif");
ImageGif($myimage);
ImageDestroy($myimage);
?>


