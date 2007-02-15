<?php
// Add physical host
// $Id: host_add.php,v 2.2 2007-02-15 12:07:11 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
// }}}

if(isset($_REQUEST['action'])) {
  if($_REQUEST['action'] == 'compou') {
    // {{{ Add computer org unit
    $dn = pql_get_define("PQL_ATTR_OU").'=Computers,'.urldecode($_REQUEST["suffix"]);
    $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = 'organizationalUnit';
    $entry[pql_get_define("PQL_ATTR_OU")] = 'Computers';

    if($_pql->add($dn, $entry, 'unit', 'host_add')) {
      // Set USER_SEARCH_DN_CTR
      $_SESSION["USER_SEARCH_DN_CTR"] = $dn;

      // Call ourself again to get the 'Add New Computer' form...
      if(!pql_get_define("PQL_CONF_DEBUG_ME"))
	pql_header($_SERVER["PHP_SELF"]);
      else
	die();
    }
// }}}
  } else {
    // {{{ Add new host
    $error = false; $error_text = array();
    $num = "(\\d|[1-9]\\d|1\\d\\d|2[0-4]\\d|25[0-5])";
    if(!isset($_REQUEST['hostip']) || !preg_match("/^$num\\.$num\\.$num\\.$num$/", $_REQUEST['hostip'])) {
      $error = true;
      $error_text["hostip"] = $LANG->_("Invalid IP address");
    }
    
    if(!isset($_REQUEST['hostname']) || !preg_match("/\w\\.\w/i", $_REQUEST['hostname'])) {
      $error = true;
      $error_text["hostname"] = $LANG->_("Invalid hostname");
    } else {
      // Inputs look good, lets add them
      if(pql_add_computer($_REQUEST['hostname'], $_REQUEST['hostip'])) {
	$msg = pql_complete_constant($LANG->_("Host %host% added successfully."), array('host' => $_REQUEST['hostname']));
	$url = 'host_detail.php?host=' . urlencode(pql_get_define("PQL_ATTR_CN") . "=" . $_REQUEST['hostname'] . "," . $_SESSION["USER_SEARCH_DN_CTR"]).
	  "&msg=$msg&rlnb=1";
      } else {
	$msg = pql_complete_constant($LANG->_("Failed to add %host%"), array('host' => $_REQUEST['hostname']));
	$url = 'host_detail.php?host=Global';
      }
      
      if(!@empty($_REQUEST["dns"])) {
	// {{{ Create the DNS record
      echo "<b>TODO: Create the DNS record</b><p>";
// }}}
      }
      
      if(pql_get_define("PQL_CONF_DEBUG_ME")) {
	echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url: '<b>$url</b>'<br>";
	die($msg);
      } else
	pql_header($url);
    }
// }}} 
  }
}

// {{{ Check if 'ou=Computers,<rootdn>' exists
$exists = 0;
if(!$_SESSION["USER_SEARCH_DN_CTR"]) {
  // Not set - find it.
  foreach($_pql->ldap_basedn as $dn)  {
    $dn = pql_format_normalize_dn($dn);
    $tmp = $_pql->get_dn($dn,
		      '(&('.pql_get_define("PQL_ATTR_OU").'=Computers)('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=organizationalUnit))',
		      'ONELEVEL');
    if(@$tmp[0]) {
      $_SESSION["USER_SEARCH_DN_CTR"] = $tmp[0];
      $exists = 1;
    }
  }
} else {
  // Make sure that the object really exists...
  $tmp = $_pql->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
		    '(&('.pql_get_define("PQL_ATTR_OU").'=Computers)('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=organizationalUnit))',
		    'BASE');
  if(@$tmp[0])
    $exists = 1;
}

if(!$exists) {
  // Output a form to offer to create 
  require($_SESSION["path"]."/header.html");
?>
    <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="1" cellpadding="3" border="0">
        <th colspan="3" align="left"><?=$LANG->_("Add New Computer organizational unit")?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title" align="center"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
            <td><?=$LANG->_("Unfortunatly I can't find the computer organizational unit<br>in any of the LDAP suffixes defined. I can't add a host until<br>this is created.")?></td>
          </tr>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_("Create ou=Computers in root DN")?></td>

            <td>
<?php	$i=0;
	foreach($_pql->ldap_basedn as $dn) {
	  $dn = pql_format_normalize_dn($dn);
?>
              <input type="radio" name="suffix" value="<?=urlencode($dn)?>"<?php if($i==0) { echo " CHECKED"; }?>><?=$dn?><br>
<?php	  $i++;
	} ?>
            </td>
          </tr>
<?php } else { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title" align="center"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
            <td><?=$LANG->_("Unfortunatly you're not super admin, so I can't create it for you.")?></td>
          </tr>
<?php } ?>
        </th>
      </table>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>

      <input type="hidden" name="action" value="compou">
      <input type="submit" name="Submit" value="<?=$LANG->_('Add New organizationalUnit')?>">
<?php } ?>
    </form>
<?php
  die();
}
// }}}

// {{{ Create the 'Add New Computer' form
require($_SESSION["path"]."/header.html");
?>
    <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="1" cellpadding="3" border="0">
        <th colspan="3" align="left"><?=$LANG->_("Add New Computer")?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_("Fully Qualified Domain Name")?></td>
            <td>
              <input type="text" name="hostname" size="40"<?php if(empty($error_text["hostname"])) { ?> value="<?=$_REQUEST['hostname']?>"<?php }?>>
              <?php if(!empty($error_text["hostname"])) { echo pql_format_error_span($error_text["hostname"]); } ?>
            </td>
          </tr>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_("IP Address")?></td>
            <td>
              <input type="text" name="hostip" size="20"<?php if(empty($error_text["hostip"])) { ?> value="<?=$_REQUEST['hostip']?>"<?php }?>>
              <?php if(!empty($error_text["hostip"])) { echo pql_format_error_span($error_text["hostip"]); } ?>
            </td>
          </tr>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_('Create DNS object')?></td>
            <td><input type="checkbox" name="dns"<?php if(!@empty($_REQUEST["dns"])) { echo "CHECKED"; } ?>>&nbsp;<?=$LANG->_('Yes')?></td>
          </tr>
        </th>
      </table>

      <input type="hidden" name="view"   value="hostacl">
      <input type="hidden" name="action" value="add_host">
      <input type="submit" name="Submit" value="<?=$LANG->_('Add New Host')?>">
    </form>
<?php
// }}}

/* Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
