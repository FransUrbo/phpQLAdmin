<?php
// Add a new mailserver to the database
// $Id: control_add_server.php,v 2.24 2005-02-24 17:04:00 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

if(pql_get_define("PQL_CONF_CONTROL_USE")) {
    // include control api if control is used
    include($_SESSION["path"]."/include/pql_control.inc");
    $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

    include($_SESSION["path"]."/header.html");

	if(isset($_REQUEST["submit"])) {
		if($_REQUEST["submit"] == "Create") {
			if($_REQUEST["fqdn"] and $_REQUEST["defaultdomain"] and $_REQUEST["ldapserver"] and $_REQUEST["ldapbasedn"]) {
				// We're ready to add the server object
				if($_REQUEST["fqdn"])
				  $_REQUEST["fqdn"] = pql_maybe_idna_encode($_REQUEST["fqdn"]);

				// Create an 'LDIF' that we can add to the databaes.
				$dn = pql_get_define("PQL_ATTR_CN").'='.$_REQUEST["fqdn"].','.$_REQUEST["ldapbasedn"];
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
				if($_REQUEST["ldaprebind"])
				  $entry[pql_get_define("PQL_ATTR_LDAPREBIND")]				= $_REQUEST["ldaprebind"];
				$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")]				= 'qmailControl';
				
				if(pql_write_add($_pql_control->ldap_linkid, $dn, $entry, 'qmail', 'control_add_server.php')) {
					$msg = urlencode("Successfully created mailserver ".$_REQUEST["fqdn"].".");
					$url = "control_detail.php?mxhost=".$_REQUEST["fqdn"]."&msg=$msg&rlnb=2";

					header("Location: " . $_SESSION["URI"] . $url);
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
		} elseif($_REQUEST["submit"] == "Clone") {
			if($_REQUEST["fqdn"]) {
				// Retreive the 'original' (object to be cloned).
				$dn = pql_get_define("PQL_ATTR_CN").'='.$_REQUEST["cloneserver"].','.$_SESSION["USER_SEARCH_DN_CTR"];
				$object = pql_search($_pql_control->ldap_linkid, $dn, 'objectclass=*', 'BASE');
				if($object) {
					// pql_search() is retreiving a too deep array. The one we're looking for is at 'position' 0
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
							} elseif((($key == pql_get_define("PQL_ATTR_LDAPPASSWORD"))
									  or
									  ($key == pql_get_define("PQL_ATTR_LDAPLOGIN")))
									 and
									 !isset($_REQUEST["include_password"])) {
								next;
							} elseif($key == pql_get_define("PQL_ATTR_CN")) {
								$entry[$key] = $_REQUEST["fqdn"];
							} elseif($key != 'dn')
							  $entry[$key][] = $val;
						}
					}
				}
				
				// Add the new object to the database.
				$dn	= pql_get_define("PQL_ATTR_CN").'='.$_REQUEST["fqdn"].','.$_SESSION["USER_SEARCH_DN_CTR"];
				if(pql_write_add($_pql_control->ldap_linkid, $dn, $entry, 'qmail', 'control_add_server.php')) {
					$msg = urlencode("Successfully created mailserver ".pql_maybe_idna_decode($_REQUEST["fqdn"]).".");
					$url = "control_detail.php?mxhost=".$_REQUEST["fqdn"]."&msg=$msg&rlnb=2";

					header("Location: " . $_SESSION["URI"] . $url);
				} else
				  die("Failed to clone QmailLDAP/Control object <b>".$_REQUEST["fqdn"]."</b>.");
			} else
			  $error_text["fqdn_clone"] = 'missing';
		}
	}
?>
  <span class="title1">Create mailserver controls object in LDAP server <?=$_SESSION["USER_HOST"]?></span>

  <table cellspacing="0" cellpadding="3" border="0">
    <th>
      <tr class="<?php pql_format_table(); ?>">
        <td><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
        <td>You can either CLONE an existing server (<u>without</u> the <?php echo pql_get_define("PQL_ATTR_LOCALS");?>/<?php echo pql_get_define("PQL_ATTR_RCPTHOSTS"); ?> information)<br><b>or</b> CREATE a new server object.</td>
      </tr>
    </th>
  </table>

  <br><br>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Clone mailserver object</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">Fully qualified domain name<br>of new mail server</td>
          <td><?php echo pql_format_error_span($error_text["fqdn_clone"]); ?>
            <input type="text" name="fqdn" value="<?=$_REQUEST["fqdn"]?>" size="30">
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title">Clone server</td>
          <td>
<?php
	// Get all QmailLDAP/Control hosts.
    $result = pql_get_dn($_pql_control->ldap_linkid,
						 $_SESSION["USER_SEARCH_DN_CTR"],
						 '(&(cn=*)(objectclass=qmailControl))',
						 'ONELEVEL');
    for($i=0; $result[$i]; $i++)
      $hosts[] = pql_get_attribute($_pql_control->ldap_linkid, $result[$i], pql_get_define("PQL_ATTR_CN"));

	if(!is_array($hosts)) {
?>
            no LDAP control hosts defined
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
          <td><input type="checkbox" name="include_locals">Include all <u><?php echo pql_get_define("PQL_ATTR_LOCALS"); ?></u> information in clone</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"></td>
          <td><input type="checkbox" name="include_rcpthosts">Include all <u><?php echo pql_get_define("PQL_ATTR_RCPTHOSTS");?></u> information in clone</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"></td>
          <td><input type="checkbox" name="include_password">Include <u><?php echo pql_get_define("PQL_ATTR_LDAPPASSWORD");?></u> in clone</td>
        </tr>
      </th>
    </table>

    <input type="submit" name="submit" value="Clone">
  </form>

  <br>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Create mailserver object</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">Fully qualified domain name</td>
          <td><?php echo pql_format_error_span($error_text["fqdn_create"]); ?>
            <input type="text" name="fqdn" value="<?=$_REQUEST["fqdn"]?>" size="30">
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title">Default domainname</td>
          <td><?php echo pql_format_error_span($error_text["defaultdomain"]); ?>
              <input type="text" name="defaultdomain" value="<?=$_REQUEST["defaultdomain"]?>" size="30">
          </td>
        </tr>
  
<?php
	if($_SESSION["ADVANCED_MODE"]) {
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">Plusdomain</td>
          <td><input type="text" name="plusdomain" value="<?=$_REQUEST["plusdomain"]?>" size="30">
        </tr>
  
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">LDAP Server</td>
          <td><?php echo pql_format_error_span($error_text["ldapserver"]); ?>
              <input type="text" name="ldapserver" value="<?=$_REQUEST["ldapserver"]?>" size="30">
          </td>
        </tr>
  
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">LDAP Search base</td>
          <td><?php echo pql_format_error_span($error_text["ldapbasedn"]); ?>
              <input type="text" name="ldapbasedn" value="<?=$_REQUEST["ldapbasedn"]?>" size="30">
          </td>
        </tr>
  
<?php if(!$_SESSION["NEW_STYLE_QUOTA"])  { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">Default quota</td>
          <td><input type="text" name="ldapdefaultquota" value="<?=$_REQUEST["ldapdefaultquota"]?>" size="30">
        </tr>
  
<?php } else { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">Default quota size</td>
          <td><input type="text" name="ldapdefaultquotasize" value="<?=$_REQUEST["ldapdefaultquotasize"]?>" size="30">
        </tr>
  
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">Default quota count</td>
          <td><input type="text" name="ldapdefaultquotacount" value="<?=$_REQUEST["ldapdefaultquotacount"]?>" size="30">
        </tr>
  
<?php } ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">Default DOT mode</td>
          <td><input type="text" name="ldapdefaultdotmode" value="<?=$_REQUEST["ldapdefaultdotmode"]?>" size="30">
        </tr>
  
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">Directory maker</td>
          <td><input type="text" name="dirmaker" value="<?=$_REQUEST["dirmaker"]?>" size="30">
        </tr>
  
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">Default quota warning</td>
          <td><input type="text" name="quotawarning" value="<?=$_REQUEST["quotawarning"]?>" size="30">
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title">LDAP Rebind</td>
          <td>
            <select name="ldaprebind">
              <option value="1" SELECTED>Yes</option>
              <option value="0">No</option>
            </select>
        </tr>
<?php
	} else {
	// This won't really happen, since we're not getting this form if we haven't
	// ADVANCED_MODE set! But anyway, just incase I find a better way later :)
?>
        <input type="hidden" name="plusdomain"            value="<?=$_REQUEST["plusdomain"]?>">
        <input type="hidden" name="ldapserver"            value="<?=$_SESSION["USER_HOST"]?>">
        <input type="hidden" name="ldapbasedn"            value="<?=$_SESSION["USER_HOST"]?>">
        <input type="hidden" name="ldapdefaultquota"      value="<?=$_REQUEST["ldapdefaultquota"]?>">
        <input type="hidden" name="ldapdefaultquotacount" value="<?=$_REQUEST["ldapdefaultquotacount"]?>">
        <input type="hidden" name="ldapdefaultquotasize"  value="<?=$_REQUEST["ldapdefaultquotasize"]?>">
        <input type="hidden" name="ldapdefaultdotmode"    value="ldapwithprog">
        <input type="hidden" name="dirmaker"              value="<?=$_REQUEST["dirmaker"]?>">
        <input type="hidden" name="quotawarning"          value="User is above quota level!">
        <input type="hidden" name="ldaprebind"            value="1">
<?php
	}
?>
      </th>
    </table>

    <input type="submit" name="submit" value="Create">
  </form>
<?php
}
// else - PQL_CONF_CONTROL_USE not set

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
  </body>
</html>
