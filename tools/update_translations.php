<?php // -*- PHP -*-
// ----------------------------
// pql_update_translations.php
//
// $Id: update_translations.php,v 2.7 2005-09-22 08:45:39 turbo Exp $
//

// ----------------------------
// This file is stolen from the phpRecipeBook project by 
// Todd Rogers <nazgul26@users.sourceforge.net>
// http://sourceforge.net/projects/phprecipebook
//
// Adopted to suit phpQLAdmin by Turbo Fredriksson
// Function and class name(s) have been altered to
// be more in line with the rest of the phpQLAdmin
// package.

require("../include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_translations.inc");

$outputFile = "translations/lang.new.inc";
$baseDir = ".\/"; // were we start searching for strings
	
$user_lang = isset($_POST['user_lang']) ? $_POST['user_lang'] : '';

// Set the OS type, only Linux and Windows known to work for sure
$os_name = pql_get_os_name();
if($os_name == "unix") { //matches most unixes
  $pathSeparator = "/";
  $baseDir = ".\/"; // were we start searching for strings
} else if($os_name == "windows") {
  $pathSeparator = "\\";
  $baseDir = "."; // were we start searching for strings
}
?>
<html>
  <head>
    <link rel="stylesheet" href="tools/normal.css" type="text/css">
  </head>

  <body>
<?php if (empty($_REQUEST['total_keys'])) { ?>
  Before saving the translation, make sure the directory 'translations'
  exists in the phpQLAdmin root and that it's writable by the webserver.
  <p>  
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="POST">
    <input type="hidden" name="mode" value="select">
<?php
	if($user_lang)
	  // read from the form
	  $lang = $user_lang;
	else
	  $lang = $LANG->_choosen;
?>
    <select class="field_listbox" name="user_lang" size="1">
<?php $supp = $LANG->get_supported();
	  while(list($k, $v) = each($supp)) {
?>
      <option value="<?=$k?>"><?=$v?></option>
<?php  } ?>
    </select>
<?php
	// Now we should load up the language file select by the user or by browser detection
	$fp = @fopen($_SESSION["path"]."/include/lang.$lang.inc", "r");
	if(!$fp) {
	  die("<p>Can't open language file 'include/lang.$lang.inc' for read. Does it exists?");
	}
	require($_SESSION["path"]."/include/lang.$lang.inc");
	$buffer = fgets($fp, 4096); // chop off the php start part
	$read = true;
	$file_text = "";
	while($read) {
		$buffer = fgets($fp, 4096);
		if(preg_match("/the alphabet of the language/", $buffer, $matches))
		  $read=false;
		else
		  $file_text .= $buffer;
	}
	fclose ($fp);
?>
  <input type=submit value="Load File">
</form>

<form action="<?=$_SERVER["PHP_SELF"]?>" method=POST>
  <input type="hidden" name="mode" value="save">
  <input type="hidden" name="lang" value="<?php echo $lang;?>">
  <input type="submit" value="Save Translation"><p>
  <textarea rows=10 cols=80 name="toptext">
<?php echo $file_text;?>

// This is the alphabet of the language. Uppercased characters only at
// this time. PHP should be able to figure out the lowercased eqvivalent.
$alphabet = array("A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K",
                  "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V",
                  "W", "X", "Y", "Z");

// If you want HTML tags, you can have that in the translation string
// (ie, the value AFTER the '=>' character).
</textarea>

  <p>

<?php
// "
	if((bool)ini_get('safe_mode') or ($os_name=="windows")) {
	  // have to use PHP functions for find and grep, this is slower
	  find($baseDir, $files, '/.*?\.php$/', $pathSeparator);
	  $array = grep($files, "/\->_\(\'.*?\'\)/");
	} else {
	  // We can use find and grep
echo "do 'find - exec'<br>";
	  $return = shell_exec("find $baseDir -follow -name \"*.php\" -exec egrep \"\->_\(\'.*?\'\)\" {} /dev/null \\;");
	  $array = preg_split("/$baseDir/", $return);
	}
echo "Return: '$return'<br>";

	$array  = preg_split ("/$baseDir/", $return);
	$keys   = array();

	foreach($array as $h) {
		preg_match_all("/\->_\('(.*?)'\)/", $h, $matches);
		for($i=1; $i < count($matches); $i++) {
			if(count($matches[$i]) > 1) {
				for($j=0; $j < count($matches[$i]); $j++)
				  $keys[$matches[$i][$j]] = "";
			} elseif(isset($matches[$i][0]))
			  $keys[$matches[$i][0]] = "b";
		}
	}

	$keys = array_keys($keys);
	sort($keys);
	$count=0;
?>
  <span class="title1">Legend:</span><br>
  <font color=red>*</font>&nbsp means a untranslated string<br>
  <font color=red><u>\n</u></font> will be replaced by a &lt;br&gt; (a HTML newline)<br><br>

  <font color=red><u>\u</u></font> will be replaced by a &lt;u&gt;  (beginning of a HTML underline)<br>
  <font color=red><u>\U</u></font> will be replaced by a &lt;/u&gt; (end of a HTML underline)<br>
  <font color=red><u>\i</u></font> will be replaced by a &lt;i&gt;  (beginning of a HTML italic)<br>
  <font color=red><u>\I</u></font> will be replaced by a &lt;/i&gt; (end of a HTML italic)<br>
  <font color=red><u>\b</u></font> will be replaced by a &lt;b&gt;  (beginning of a HTML bold)<br>
  <font color=red><u>\B</u></font> will be replaced by a &lt;/b&gt; (end of a HTML bold)<br><br>

  All entries between % will be translated within the code (replaced with a value). Make sure they are kept.
  <p>

  <table border=1 cellpadding=2 cellspacing=2>
<?php
	foreach($keys as $key) {
		if($key != "") {
			$key = ereg_replace("\\\\'", "'", $key);
			echo "<tr><td>";

			if($language[$key] == "") 
			  echo "<font color=red>*</font> ";

			echo $key . "\n";
			echo "<input type=hidden name=\"key_$count\" size=50 value=\"$key\"></td>";
			echo "<td><input type=text name=\"value_$count\" size=50 value=\"$language[$key]\"></td></tr>\n";
			$count++;
		}
	}
?>
  </table>

  <input type="hidden" name="total_keys" value="<?=$count-1?>">
  <input type="submit" value="Save Translation"><p>
</form>
<?php } else { 
	// they have submitted the form	
	$lang	 = $_REQUEST['lang'];
	$header  = "<?php\n";
	$header .= $_REQUEST['toptext']."\n";
	$header  = stripslashes($header);

	// Remove any M$ newlines...
	$header  = eregi_replace("", "", $header);

	$total   = $_REQUEST['total_keys'];

	// Prevent refresh from aborting file
	ignore_user_abort(true);

	$fp      = fopen($outputFile, "w");
	if(!$fp) {
	  ignore_user_abort(false);
	  die("<p>Can't open language file '$outputFile' for write. Does the directory exists and is writable by the webserver?");
	}

	fputs($fp, $header);
	for($i=0; $i <= $total; $i++) {
		$key = "key_$i";
		$val = "value_$i";

		if($i==0)
			$str = '$language = array("'.$_REQUEST[$key].'" => "'.$_REQUEST[$val].'\"';
		else
			$str = '				  "'.$_REQUEST[$key].'" => "'.$_REQUEST[$val].'\"';

		$next = $i+1; $next = "key_$next";
		if($_REQUEST[$next])
			$str .= ",\n";
		else
			$str .= ");\n\n";

		if(!empty($str)) {
			$str = stripslashes($str);
			fputs($fp, $str);
		}
	}

	// Put EMACS mode stuff at the very end...
	fputs($fp, "/*\n");
	fputs($fp, " * Local variables:\n");
	fputs($fp, " * mode: php\n");
	fputs($fp, " * tab-width: 4\n");
	fputs($fp, " * End:\n");
	fputs($fp, " */\n?>\n");

	fclose($fp);

	echo "Translation saved as new file: $outputFile<BR>";

	// Put things back to normal
	ignore_user_abort(false);
}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
