<?php
// Add a new mailserver to the database
// $Id: control_add_server.php,v 2.35 2007-02-15 12:07:09 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
// }}}

if(pql_get_define("PQL_CONF_CONTROL_USE")) {
  // {{{ Include control api if control is used
  include($_SESSION["path"]."/include/pql_control.inc");
  $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

  include($_SESSION["path"]."/header.html");
// }}}

  if(isset($_REQUEST["submit"])) {
	if($_REQUEST["submit"] == "Clone") {
	  // {{{ Clone mail server
	  if($_REQUEST["fqdn"]) {
		// Retreive the DN of the 'original' (object to be cloned).
		$filter = '(&('.pql_get_define("PQL_ATTR_CN").'='.$_REQUEST["cloneserver"].')('.pql_get_define("PQL_ATTR_OBJECTCLASS").'=qmailControl))';
		$dn = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"], $filter);
		if($dn[0])
		  $dn = $dn[0];

		// Get the whole object
		$object = $_pql_control->search($dn, 'objectclass=*', 'BASE');
		if($object) {
		  if($object[0])
			// search() is retreiving a too deep array. The one we're looking for is at 'position' 0
			$object = $object[0];
		  
		  // Create an 'LDIF' for the new object, that we can add to the databaes.
		  foreach($object as $key => $tmp) {
			if(!is_array($tmp))
			  // Make the attribute value an array. Simpler than duplicating code below...
			  $value = array($tmp);
			else
			  // It's already an array. Good.
			  $value = $tmp;
			
			foreach($value as $val) {
			  if(($key == pql_get_define("PQL_ATTR_RCPTHOSTS")) or ($key == pql_get_define("PQL_ATTR_LOCALS"))) {
				// {{{ rcptHosts or locals
				if(($key == pql_get_define("PQL_ATTR_RCPTHOSTS")) and isset($_REQUEST["include_rcpthosts"])) {
				  if($val == $_REQUEST["cloneserver"]) {
					// rcptHosts have the FQDN of the cloned server -
					// Replace that with the FQDN of the resulting server.
					$entry[$key][] = ereg_replace($_REQUEST["cloneserver"], $_REQUEST["fqdn"], $val);
				  } else
					$entry[$key][] = $val;
				} elseif(($key == pql_get_define("PQL_ATTR_LOCALS")) and isset($_REQUEST["include_locals"])) {
				  if($val == $_REQUEST["cloneserver"]) {
					// locals have the FQDN of the cloned server -
					// Replace that with the FQDN of the resulting server.
					$entry[$key][] = ereg_replace($_REQUEST["cloneserver"], $_REQUEST["fqdn"], $val);
				  } else
					$entry[$key][] = $val;
				}
// }}}
			  } elseif((($key == pql_get_define("PQL_ATTR_LDAPPASSWORD")) or
						($key == pql_get_define("PQL_ATTR_LDAPLOGIN")))
					   and
					   !isset($_REQUEST["include_password"])) {
				next;
			  } elseif(($key == pql_get_define("PQL_ATTR_CN")) or
					   ($key == pql_get_define("PQL_ATTR_DEFAULTHOST")) or
					   ($key == pql_get_define("PQL_ATTR_HELOHOST")))
				$entry[$key] = $_REQUEST["fqdn"];
			  elseif($key != 'dn')
				$entry[$key][] = $val;
			}
		  }
		}
		
		// Add the new object to the database.
		$dn	= pql_get_define("PQL_ATTR_CN").'='.$_REQUEST["fqdn"].','.$_REQUEST["host"];
		if($_pql_control->add($dn, $entry, 'qmail', 'control_add_server.php')) {
		  $msg  = urlencode("Successfully created mailserver ".pql_maybe_idna_decode($_REQUEST["fqdn"]).".");
		  $link = "control_detail.php?mxhost=".urlencode($dn)."&msg=$msg&rlnb=2";
		  
		  if(pql_get_define("PQL_CONF_DEBUG_ME")) {
			echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
			die("<b>$link</b>");
		  } else
			pql_header($link);
		} else
		  die("Failed to clone QmailLDAP/Control object <b>".$_REQUEST["fqdn"]."</b>.");
	  } else
		$error_text["fqdn_clone"] = 'missing';
// }}}
	} elseif($_REQUEST["submit"] == "Create") {
	  // {{{ Create mail server
	  if($_REQUEST["fqdn"] and $_REQUEST["defaultdomain"] and $_REQUEST["ldapserver"] and $_REQUEST["ldapbasedn"]) {
		// We're ready to add the server object
		if($_REQUEST["fqdn"])
		  $_REQUEST["fqdn"] = pql_maybe_idna_encode($_REQUEST["fqdn"]);
		
		// Create an 'LDIF' that we can add to the databaes.
		$dn = pql_get_define("PQL_ATTR_CN").'='.$_REQUEST["fqdn"].','.$_REQUEST["host"];
		$entry[pql_get_define("PQL_ATTR_CN")] = $_REQUEST["fqdn"];
		if($_REQUEST["defaultdomain"])
		  $entry[pql_get_define("PQL_ATTR_DEFAULTDOMAIN")]			= $_REQUEST["defaultdomain"];
		if($_REQUEST["plusdomain"])
		  $entry[pql_get_define("PQL_ATTR_PLUSDOMAIN")]				= $_REQUEST["plusdomain"];
		if($_REQUEST["ldapserver"])
		  $entry[pql_get_define("PQL_ATTR_LDAPSERVER")]				= $_REQUEST["ldapserver"];
		if($_REQUEST["ldapbasedn"])
		  $entry[pql_get_define("PQL_ATTR_LDAPBASEDN")]				= $_REQUEST["ldapbasedn"];
		if($_REQUEST["ldapdefaultquotasize"])
		  $entry[pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA_SIZE")]	= $_REQUEST["ldapdefaultquotasize"];
		if($_REQUEST["ldapdefaultquotacount"])
		  $entry[pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA_COUNT")]	= $_REQUEST["ldapdefaultquotacount"];
		if($_REQUEST["ldapdefaultdotmode"])
		  $entry[pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE")]		= $_REQUEST["ldapdefaultdotmode"];
		if($_REQUEST["dirmaker"])
		  $entry[pql_get_define("PQL_ATTR_DIRMAKER")]				= $_REQUEST["dirmaker"];
		if($_REQUEST["quotawarning"])
		  $entry[pql_get_define("PQL_ATTR_QUOTA_WARNING")]			= $_REQUEST["quotawarning"];
	    if(!@empty($_REQUEST["ldaprebind"]))
		  $entry[pql_get_define("PQL_ATTR_LDAPREBIND")]				= '1';
		$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")]				= 'qmailControl';
		
		if($_pql_control->add($dn, $entry, 'qmail', 'control_add_server.php')) {
		  $msg  = urlencode("Successfully created mailserver ".$_REQUEST["fqdn"].".");
		  $link = "control_detail.php?mxhost=".urlencode($dn)."&msg=$msg&rlnb=2";
		  
		  if(pql_get_define("PQL_CONF_DEBUG_ME")) {
			echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
			die("<b>$link</b>");
		  } else
			pql_header($link);
		} else
		  die("Failed to add QmailLDAP/Control object <b>".$_REQUEST["fqdn"]."</b>");
	  } else {
		if(! $_REQUEST["fqdn"])
		  $error_text["fqdn_create"] = 'missing';
		if(! $_REQUEST["defaultdomain"])
		  $error_text["defaultdomain"] = 'missing';
		if(! $_REQUEST["ldapserver"])
		  $error_text["ldapserver"] = 'missing';
		if(! $_REQUEST["ldapbasedn"])
		  $error_text["ldapbasedn"] = 'missing';
	  }
// }}}
	}
  }

  // Retreive the FQDN of physical host
  $server_reference = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));

  // {{{ Create the 'Clone mail server' form
?>
  <span class="title1">Create mail server controls object for physical host <?=$server_reference?></span>

  <table cellspacing="0" cellpadding="3" border="0">
    <th>
      <tr class="<?php pql_format_table(); ?>">
        <td><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
        <td><?=pql_complete_constant($LANG->_('You can either CLONE an existing server (\uwith\U or \uwithout\U the %attribs%\ninformation) or create a new server object.'), array('attribs' => pql_get_define("PQL_ATTR_LOCALS").'/'. pql_get_define("PQL_ATTR_RCPTHOSTS")))?></td>
      </tr>
    </th>
  </table>

  <br><br>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Clone mailserver object')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Fully qualified domain name')?></td>
          <td><?php if(!empty($error_text["fqdn_clone"])) { echo pql_format_error_span($error_text["fqdn_clone"]); }?>
            <input type="text" name="fqdn" <?php if(!empty($_REQUEST["fqdn"])) { echo 'value="'.$_REQUEST["fqdn"].'"'; } else { echo "value=$server_reference"; } ?> size="30">
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Clone server')?></td>
          <td>
<?php
  // Get all QmailLDAP/Control hosts.
  $result = $_pql_control->get_dn(
					   $_SESSION["USER_SEARCH_DN_CTR"],
					   '(&(cn=*)(objectclass=qmailControl))');
  for($i=0; $i < count($result); $i++)
	$hosts[] = $_pql_control->get_attribute($result[$i], pql_get_define("PQL_ATTR_CN"));

  if(!is_array($hosts)) {
?>
            <?=$LANG->_('no LDAP control hosts defined')?>
<?php
  } else {
?>
            <select name="cloneserver">
<?php
	// for each host, get LDAP/Control plugins
	foreach($hosts as $host) {
?>
              <option value="<?=$host?>"><?=$host?></option>
<?php
	}
?>
            </select>
<?php
}
?>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"></td>
          <td>
            <input type="checkbox" name="include_locals" CHECKED>
            <?=pql_complete_constant($LANG->_('Include all \u%attrib%\U information in clone'), array('attrib' => pql_get_define("PQL_ATTR_LOCALS")))?>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"></td>
          <td>
            <input type="checkbox" name="include_rcpthosts" CHECKED>
            <?=pql_complete_constant($LANG->_('Include all \u%attrib%\U information in clone'), array('attrib' => pql_get_define("PQL_ATTR_RCPTHOSTS")))?>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"></td>
          <td>
            <input type="checkbox" name="include_password" CHECKED>
            <?=pql_complete_constant($LANG->_('Include all \u%attrib%\U information in clone'), array('attrib' => pql_get_define("PQL_ATTR_LDAPPASSWORD")))?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="host"   value="<?=$_REQUEST["host"]?>">
    <input type="submit" name="submit" value="<?=$LANG->_('Clone')?>">
  </form>
<?php
// }}}
?>

  <br>

<?php
  // {{{ Create the 'Create mail server' form
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Create new mailserver object')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Fully qualified domain name')?></td>
          <td><?php if(!empty($error_text["fqdn_create"])) { echo pql_format_error_span($error_text["fqdn_create"]); }?>
            <input type="text" name="fqdn" <?php if(!empty($_REQUEST["fqdn"])) { echo 'value="'.$_REQUEST["fqdn"].'"'; } else { echo "value=$server_reference"; } ?> size="30">
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Default domainname')?></td>
          <td><?php if(!empty($error_text["defaultdomain"])) { echo pql_format_error_span($error_text["defaultdomain"]); }?>
              <input type="text" name="defaultdomain" <?php if(!empty($_REQUEST["defaultdomain"])) { echo 'value="'.$_REQUEST["defaultdomain"].'"'; }?> size="30">
          </td>
        </tr>
  
<?php
  if($_SESSION["ADVANCED_MODE"]) {
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Plusdomain')?></td>
          <td><input type="text" name="plusdomain" <?php if(!empty($_REQUEST["plusdomain"])) { echo 'value="'.$_REQUEST["plusdomain"].'"'; }?> size="30">
        </tr>
  
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('LDAP Server')?></td>
          <td><?php if(!empty($error_text["ldapserver"])) { echo pql_format_error_span($error_text["ldapserver"]); }?>
              <input type="text" name="ldapserver" <?php if(!empty($_REQUEST["ldapserver"])) { echo 'value="'.$_REQUEST["ldapserver"].'"'; }?> size="30">
          </td>
        </tr>
  
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('LDAP Search base')?></td>
          <td><?php if(!empty($error_text["ldapbasedn"])) { echo pql_format_error_span($error_text["ldapbasedn"]); }?>
              <input type="text" name="ldapbasedn" <?php if(!empty($_REQUEST["ldapbasedn"])) { echo 'value="'.$_REQUEST["ldapbasedn"].'"'; }?> size="30">
          </td>
        </tr>
  
<?php if(!$_SESSION["NEW_STYLE_QUOTA"])  { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Default quota')?></td>
          <td><input type="text" name="ldapdefaultquota" <?php if(!empty($_REQUEST["ldapdefaultquota"])) { echo 'value="'.$_REQUEST["ldapdefaultquota"].'"'; }?> size="30">
        </tr>
  
<?php } else { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Default quota size')?></td>
          <td><input type="text" name="ldapdefaultquotasize" <?php if(!empty($_REQUEST["ldapdefaultquotasize"])) { echo 'value="'.$_REQUEST["ldapdefaultquotasize"].'"'; }?> size="30">
        </tr>
  
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Default quota count')?></td>
          <td><input type="text" name="ldapdefaultquotacount" <?php if(!empty($_REQUEST["ldapdefaultquotacount"])) { echo 'value="'.$_REQUEST["ldapdefaultquotacount"].'"'; }?> size="30">
        </tr>
  
<?php } ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Default DOT mode')?></td>
          <td>
            <select name="ldapdefaultdotmode">
              <option value="both"><?=$LANG->_('both')?></option>
              <option value="dotonly"><?=$LANG->_('dotonly')?></option>
              <option value="ldaponly" SELECTED><?=$LANG->_('ldaponly')?></option>
              <option value="ldapwithprog"><?=$LANG->_('ldapwithprog')?></option>
            </select>
          </td>
        </tr>
  
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Directory maker')?></td>
          <td><input type="text" name="dirmaker" <?php if(!empty($_REQUEST["dirmaker"])) { echo 'value="'.$_REQUEST["dirmaker"].'"'; }?> size="30">
        </tr>
  
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Default quota warning')?></td>
          <td><input type="text" name="quotawarning" <?php if(!empty($_REQUEST["quotawarning"])) { echo 'value="'.$_REQUEST["quotawarning"].'"'; }?> size="30">
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('LDAP Rebind')?></td>
          <td><input type="checkbox" name="ldaprebind" CHECKED>&nbsp;<?=$LANG->_('Yes')?></td>
        </tr>
<?php
  } else {
	// This won't really happen, since we're not getting this form if we haven't
	// ADVANCED_MODE set! But anyway, just incase I find a better way later :)
?>
        <input type="hidden" name="plusdomain"            <?php if(!empty($_REQUEST["plusdomain"])) { echo 'value="'.$_REQUEST["plusdomain"].'"'; }?>>
        <input type="hidden" name="ldapserver"            <?php if(!empty($_REQUEST["USER_HOST"])) { echo 'value="'.$_SESSION["USER_HOST"].'"'; }?>>
        <input type="hidden" name="ldapbasedn"            <?php if(!empty($_REQUEST["USER_HOST"])) { echo 'value="'.$_SESSION["USER_HOST"].'"'; }?>>
        <input type="hidden" name="ldapdefaultquota"      <?php if(!empty($_REQUEST["ldapdefaultquota"])) { echo 'value="'.$_REQUEST["ldapdefaultquota"].'"'; }?>>
        <input type="hidden" name="ldapdefaultquotacount" <?php if(!empty($_REQUEST["ldapdefaultquotacount"])) { echo 'value="'.$_REQUEST["ldapdefaultquotacount"].'"'; }?>>
        <input type="hidden" name="ldapdefaultquotasize"  <?php if(!empty($_REQUEST["ldapdefaultquotasize"])) { echo 'value="'.$_REQUEST["ldapdefaultquotasize"].'"'; }?>>
        <input type="hidden" name="ldapdefaultdotmode"    value="ldapwithprog">
        <input type="hidden" name="dirmaker"              <?php if(!empty($_REQUEST["dirmaker"])) { echo 'value="'.$_REQUEST["dirmaker"].'"'; }?>>
        <input type="hidden" name="quotawarning"          value="User is above quota level!">
        <input type="hidden" name="ldaprebind"            value="1">
<?php
  }
?>
      </th>
    </table>

    <input type="hidden" name="host"   value="<?=$_REQUEST["host"]?>">
    <input type="submit" name="submit" value="Create">
  </form>
<?php
// }}}
}
// else - PQL_CONF_CONTROL_USE not set
?>
  </body>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
