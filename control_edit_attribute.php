<?php
// edit an attribute of a control option
// $Id: control_edit_attribute.php,v 1.1 2002-12-11 15:09:23 turbo Exp $
//

// initial check
if($attrib == ""){
	die("no attribute requested !!");
}

if($type == ""){
	$type = "modify";
}

// requires
require("pql.inc");
require("pql_control.inc");
require("pql_control_plugins.inc");

$_pql = new pql();
$_pql_control = new pql_control();

// register all attribute plugins here (require)
$control_plugin = pql_control_plugin_get($attrib);
include(pql_control_plugin_get_filename($control_plugin));


// forward back to users detail page
function attribute_forward($msg){
	global $domain, $user, $attrib;

	$msg = urlencode($msg);
	$cat = pql_control_plugin_cat($attrib);
	$url = "control_cat.php?cat=" . urlencode($cat) . "&msg=$msg";
	header("Location: " . PQL_URI . "$url");
}
?>

<html>
<head>
	<title>phpQL</title>
	<link rel="stylesheet" href="normal.css" type="text/css">
</head>

<body bgcolor="#e7e7e7" background="images/bkg.png">
<span class="title1">Change control values</span>
<br>
<br>
<?php
if(function_exists($control_plugin . "_help")){
	// if a help function is available for the plugin,
	// print additional help table and put the
	// displayed form of the plugin into a cell of this
	// table
?>
<table cellspacing="0" cellpadding="3" border="0">
	<tr>
		<td width="450" valign="top">
<?php } ?>
<?php
	// select what to do
	if($submit == 1){
		if(call_user_func($control_plugin . "_check", $type)){
			call_user_func($control_plugin . "_save", $type);
		} else {
			call_user_func($control_plugin . "_print_form");
   		}
  	} else {
			call_user_func($control_plugin . "_init");
			call_user_func($control_plugin . "_print_form");
  	}
?>
<?php
if(function_exists($control_plugin . "_help")){
	// this is the help table
?>
		</td>
		<td>&nbsp;</td>
		<td valign="top">
			<table cellspacing="0" cellpadding="3" border="0">
				<tr>
					<td valign="middle" class="helptitle"><img src="images/help.png" width="16" height="16" border="0">&nbsp;help on this attribute</td>
				</tr>
				<tr>
					<td height="0" width="200" class="helptext" valign="top">
						<?php call_user_func($control_plugin . "_help"); ?>
					</td>
				</tr>
				<?php
				if(function_exists($control_plugin . "_help_cr")){
					// this is the copyright message supplied with help text
				?>
				<tr>
					<td height="0" width="200" class="helptextcr" valign="top">
						<?php call_user_func($control_plugin . "_help_cr"); ?>
					</td>
				</tr>
				<?php
				}
				?>
			</table>
		</td>
	</tr>
</table>
<?php } ?>

</body>
</html>
