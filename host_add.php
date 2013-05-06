<?php
// Add physical host
// $Id: host_add.php,v 2.4 2007-07-19 10:12:34 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");

if(pql_get_define("PQL_CONF_DEBUG_ME")) {
  echo "<pre>";
  echo "Request array: "; printr($_REQUEST);
  echo "Session array: "; printr($_SESSION);
  echo "</pre>";
}
// }}}

if(isset($_REQUEST['action'])) {
  if($_REQUEST['action'] == 'compou') {
    // {{{ Add computer org unit
    $dn = pql_get_define("PQL_CONF_SUBTREE_COMPUTERS").','.urldecode($_REQUEST["suffix"]);
    $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = 'organizationalUnit';

    $dnparts = explode('=', pql_get_define("PQL_CONF_SUBTREE_COMPUTERS"));
    $entry[pql_get_define("PQL_ATTR_OU")] = $dnparts[1];

    if($_pql->add($dn, $entry, 'unit', 'host_add')) {
      // Set USER_SEARCH_DN_CTR
      $_SESSION["USER_SEARCH_DN_CTR"] = $dn;

      // Call ourself again to get the 'Add New Computer' form...
      pql_header($_SERVER["HTTP_REFERER"], 0);
    }
// }}}
  } else {
    // {{{ Add new host
    $error = false; $error_text = array();
    $num = "(\\d|[1-9]\\d|1\\d\\d|2[0-4]\\d|25[0-5])";
    if(!isset($_REQUEST['hostip']) || (!preg_match("/^$num\\.$num\\.$num\\.$num$/", $_REQUEST['hostip']) && !preg_match('/^DHCP$/', $_REQUEST['hostip']))) {
      $error = true;
      $error_text["hostip"] = $LANG->_("Invalid IP address");
    }

    if(!isset($_REQUEST['branch'])) {
      $error = true;
      $error_text['branch'] = $LANG->_('Missing branch');
    }

    $rootdn = pql_get_rootdn($_REQUEST['branch']);
    if(!isset($_REQUEST['hostname']) || (pql_get_define("PQL_CONF_FORCE_FQDN", $rootdn) && !preg_match("/\w\\.\w/i", $_REQUEST['hostname']))) {
      $error = true;
      $error_text["hostname"] = $LANG->_("Invalid hostname");
    }

    if(!$error) {
      // Inputs look good, lets add them
      if(pql_add_computer($_REQUEST['hostname'], $_REQUEST['hostip'], $_REQUEST['branch'])) {
	$msg  = pql_complete_constant($LANG->_("Host %host% added successfully."), array('host' => $_REQUEST['hostname']));
	$host = urlencode(pql_get_define("PQL_ATTR_CN") . "=" . $_REQUEST['hostname'] . "," . $_SESSION["USER_SEARCH_DN_CTR"]);
	$url  = "host_detail.php?host=$host&msg=$msg&rlnb=1&ref=physical&server=$host&view=physical";
     } else {
	$msg = pql_complete_constant($LANG->_("Failed to add %host%"), array('host' => $_REQUEST['hostname']));
	$url = 'host_detail.php?host=Global';
      }
      
      if(!@empty($_REQUEST["dns"])) {
	// {{{ Create the DNS record
      echo "<b>TODO: Create the DNS record</b><p>";
// }}}
      }
      
      pql_header($url);
    }
// }}} 
  }
}

// {{{ Check if '<PQL_CONF_SUBTREE_COMPUTERS>,<rootdn>' exists
$exists = 0;
if(!$_SESSION["USER_SEARCH_DN_CTR"]) {
  // Not set - find it.
  foreach($_pql->ldap_basedn as $dn)  {
    $dn = pql_format_normalize_dn($dn);
    $tmp = $_pql->get_dn($dn,
		      '(&('.pql_get_define("PQL_CONF_SUBTREE_COMPUTERS").')('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=organizationalUnit))',
		      'ONELEVEL');
    if(@$tmp[0]) {
      $_SESSION["USER_SEARCH_DN_CTR"] = $tmp[0];
      $exists = 1;
    }
  }

  if(!$exists) {
    // Look in each of the branches
    $domains = pql_get_domains();
    foreach($domains as $dn)  {
      $dn = pql_format_normalize_dn($dn);
      $tmp = $_pql->get_dn($dn,
			   '(&('.pql_get_define("PQL_CONF_SUBTREE_COMPUTERS").')('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=organizationalUnit))',
			   'ONELEVEL');
      if(@$tmp[0]) {
	if($_SESSION["USER_SEARCH_DN_CTR"]) {
	  if(is_array($_SESSION["USER_SEARCH_DN_CTR"]))
	    $_SESSION["USER_SEARCH_DN_CTR"][] = $tmp[0];
	  else {
	    $tmp2 = $_SESSION["USER_SEARCH_DN_CTR"];
	    $_SESSION["USER_SEARCH_DN_CTR"] = array($tmp2, $tmp[0]);
	  }
	} else
	  $_SESSION["USER_SEARCH_DN_CTR"] = $tmp[0];

	$exists = 1;
      }
    }
  }
} else {
  // Make sure that the object really exists...
  if(is_array($_SESSION["USER_SEARCH_DN_CTR"])) {
    // Force this - can only be an array if found in this code!
    $exists = 1;
  } else {
    $tmp = $_pql->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
			 '(&('.pql_get_define("PQL_CONF_SUBTREE_COMPUTERS").')('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=organizationalUnit))',
			 'BASE');
    if(@$tmp[0])
      $exists = 1;
  }
}

if(!$exists) {
  // Output a form to offer to create 
  require($_SESSION["path"]."/header.html");
?>
    <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="1" cellpadding="3" border="0">
        <th colspan="3" align="left"><?php echo $LANG->_("Add New Computer organizational unit")?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title" align="center"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
            <td><?php echo $LANG->_("Unfortunatly I can't find the computer organizational unit<br>in any of the LDAP suffixes defined. I can't add a host until<br>this is created.")?></td>
          </tr>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>

          <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo pql_complete_constant($LANG->_("Create %object% in root DN"), array('object' => pql_get_define("PQL_CONF_SUBTREE_COMPUTERS")))?></td>

            <td>
<?php	$i=0;
	foreach($_pql->ldap_basedn as $dn) {
	  $dn = pql_format_normalize_dn($dn);
?>
              <input type="radio" name="suffix" value="<?php echo urlencode($dn)?>"<?php if($i==0) { echo " CHECKED"; }?>><?php echo $dn?><br>
<?php	  $i++;
	} ?>
            </td>
          </tr>
<?php } else { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title" align="center"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
            <td><?php echo $LANG->_("Unfortunatly you're not super admin, so I can't create it for you.")?></td>
          </tr>
<?php } ?>
        </th>
      </table>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>

      <input type="hidden" name="action" value="compou">
      <input type="submit" name="Submit" value="<?php echo $LANG->_('Add New organizationalUnit')?>">
<?php } ?>
    </form>
<?php
  die();
}
// }}}

// {{{ Create the 'Add New Computer' form
require($_SESSION["path"]."/header.html");
?>
    <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="1" cellpadding="3" border="0">
        <th colspan="3" align="left"><?php echo $LANG->_("Add New Computer")?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?php echo $LANG->_("Fully Qualified Domain Name")?></td>
            <td>
              <input type="text" name="hostname" size="40"<?php if(empty($error_text["hostname"])) { ?> value="<?php echo $_REQUEST['hostname']?>"<?php }?>>
              <?php if(!empty($error_text["hostname"])) { echo pql_format_error_span($error_text["hostname"]); } ?>
            </td>
          </tr>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?php echo $LANG->_("IP Address")?></td>
            <td>
              <input type="text" name="hostip" size="20"<?php if(empty($error_text["hostip"])) { ?> value="<?php echo $_REQUEST['hostip']?>"<?php }?>>
              <?php if(!empty($error_text["hostip"])) { echo pql_format_error_span($error_text["hostip"]); } ?><br>
              <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo $LANG->_("Can be either the word \bDHCP\B or the IP address.")?>
            </td>
          </tr>
<?php if(pql_get_define("PQL_CONF_BIND9_USE")) { ?>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?php echo $LANG->_('Create DNS object')?></td>
            <td><input type="checkbox" name="dns"<?php if(!@empty($_REQUEST["dns"])) { echo "CHECKED"; } ?>>&nbsp;<?php echo $LANG->_('Yes')?></td>
          </tr>
<?php } ?>
<?php if(is_array($_SESSION["USER_SEARCH_DN_CTR"])) { ?>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?php echo $LANG->_('Add computer to branch')?></td>
            <td>
              <select name="branch">
<?php    foreach($_SESSION["USER_SEARCH_DN_CTR"] as $dn) { ?>
                <option value="<?php echo $dn?>" <?php if($_REQUEST["branch"] == $dn) { echo SELECTED; } ?>><?php echo $dn?></option>
<?php    } ?>
              </select>
            </td>
          </tr>
<?php } else { ?>

          <input type="hidden" name="branch" value="<?php echo $_SESSION["USER_SEARCH_DN_CTR"]?>">
<?php } ?>
        </th>
      </table>

      <input type="hidden" name="view"   value="newhost">
      <input type="hidden" name="action" value="add_host">
      <input type="submit" name="Submit" value="<?php echo $LANG->_('Add New Host')?>">
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
