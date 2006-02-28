<?php
require("./include/pql_session.inc");
require($_SESSION["path"]."/left-head.html");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_control.inc");

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);
$url["unit"]   = pql_format_urls($_REQUEST["unit"]);

include($_SESSION["path"]."/header.html");

function updateSudo($ldap, $dn, $type, $attribute) {
  if(isset($_REQUEST[$type]) && $_REQUEST[$type] == '') { return; }
  if(isset($_REQUEST[$type]) && $_REQUEST[$type] == 'none') { return; }
  if(isset($_REQUEST[$type]) && $_REQUEST[$type] != 'none' &&
     pql_modify_attribute($ldap, $dn, $attribute, '', $_REQUEST[$type] ))
  {
    pql_format_status_msg("Added $type=" . $_REQUEST[$type] . " To Sudo Role: " . $dn);
  } else {
    pql_format_status_msg("Failed To Add $type=" . $_REQUEST[$type] . " To Sudo Role: " . $dn);
  }
}

function listExpand($role, $alist, $attribute, $extraURL) {
  if(is_array($alist)) {
?>
  <div id='<?php echo $role.$attribute; ?>Parent' class='parent'>
    <a href="<?php echo $_SERVER['REQUEST_URI']; ?>"
       onClick="if (capable) {expandBase('<?php echo $role.$attribute; ?>', true); return false;}">
       <img name='imEx' src='images/plus.png' border='0' alt='+' width='9'
            height='9' id='<?php echo $role.$attribute; ?>Img'>
    </a>
    <?php echo $alist[0]; ?>
    <a href="<?php echo $_SERVER['REQUEST_URI']; ?>&<?php echo $extraURL;?>&<?php echo $attribute; ?>=<?php echo $list[0]; ?>">
      <img src='images/del.png' width='12' height='12' border='0' alt="Delete <?php echo $alist[0]; ?> from Sudo role"></a><br>
  </div>

  <div id='<?php echo $role.$attribute; ?>Child' class='child'>
    <nobr>
<?php for($w = 1; $w < count($alist); $w++) {
	print "\t\t" . $alist[$w];
	print " <a href=\"" . $_SERVER['REQUEST_URI'] . $extraURL;
	print "&attribute=" . $attribute . "&" . $attribute . "=" . $alist[$w] . "\">";
	print "<img src='images/del.png' width='12' height='12' border='0' alt=\"Delete ";
	print $alist[$w] . " from Sudo role\"></a><br>\n";
      }
  } else {
    if(isset($alist)) {
      print "\t\t" . $alist;
      print " <a href=\"" . $_SERVER['REQUEST_URI'] . $extraURL;
      print "&attribute=" . $attribute . "&" . $attribute . "=" . $alist . "\">";
      print "<img src='images/del.png' width='12' height='12' border='0' alt=\"Delete ";
      print $alist . " from Sudo role\"></a>\n";
    } else {
      print "&nbsp;";
    }
  }

  print "</nobr></div>";
}

function array_push_associative(&$arr) {
  $args = func_get_args();
  foreach ($args as $arg) {
    if (is_array($arg)) {
      foreach ($arg as $key => $value) {
	$arr[$key] = $value;
	$ret++;
      }
    } else {
      $arr[$arg] = "";
    }
  }

  return $ret;
}

if(pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_HOSTACL_USE")) == 'TRUE' ) {
  $button1 = array('host' => 'Host Control');
}

if(pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_SUDO_USE")) == 'TRUE') {
  $button2 = array('sudo' => 'Sudoers Access');
}

if(isset($button1) && !isset($button2) ) {
  $new = $button1;
} elseif(isset($button2) && !isset($button1) ) {
  $new = $button2;
} elseif( isset($button1) && isset($button2) ) {
  $new = $button1;
  array_push_associative($new,  $button2);
}

if(isset($new)) {
  pql_generate_button($new, "");
}

// Create a form to add users to hosts access list, if the userdn 
// & uid are passed to this page with the view == host then 
// No drop down is used, instead the field is auto populated
if(isset($_REQUEST['view']) && $_REQUEST['view'] == 'host' && 
   pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_HOSTACL_USE")) == 'TRUE' )
{
  include($_SESSION["path"]."/host_modify.php");
} elseif(isset($_REQUEST['view']) && $_REQUEST['view'] == 'sudo' &&
	 pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_SUDO_USE")) == 'TRUE' )
{
  include($_SESSION["path"]."/sudo_modify.php");
}

?>
