<?php
// Add a new mailserver to the database
// $Id: control_add_server.php,v 2.3 2002-12-29 10:37:25 turbo Exp $
//
session_start();

require("./include/pql.inc");

if(PQL_LDAP_CONTROL_USE){
    // include control api if control is used
    include("./include/pql_control.inc");
    $_pql_control = new pql_control($USER_HOST_CTR, $USER_DN, $USER_PASS);

    include("./header.html");

	if(isset($submit)) {
		if($submit == "Create") {
			if($fqdn and $defaultdomain and $ldapserver and $ldapbasedn) {
				// We're ready to add the server object
				// TODO
				die("we're ready to <b>create</b>, but can't do anything yet!<br>");

				$msg = urlencode("Successfully created mailserver $fqdn.");
				header("Location: " . PQL_URI . "control_detail.php?host=$fqdn&msg=$msg&rlnb=1");
			} else {
				if(! $fqdn)
				  $error_text["fqdn_create"] = 'missing';
				if(! $defaultdomain)
				  $error_text["defaultdomain"] = 'missing';
				if(! $ldapserver)
				  $error_text["ldapserver"] = 'missing';
				if(! $ldapbasedn)
				  $error_text["ldapbasedn"] = 'missing';
			}
		} elseif($submit == "Clone") {
			if($fqdn) {

				// Get the values of the mailserver
				$attribs = array("defaultdomain", "plusdomain", "ldapserver",
								 "ldaprebind", "ldapbasedn", "ldapdefaultquota",
								 "ldapdefaultdotmode", "dirmaker", "quotawarning",
								 "ldapuid", "ldapgid");
				$cn = "cn=" . $cloneserver . "," . $USER_SEARCH_DN_CTR;
				foreach($attribs as $attrib) {
					$value = pql_control_get_attribute($_pql_control->ldap_linkid, $cn, $attrib);
					if(!is_null($value)) {
						for($i=0; $value[$i]; $i++) {
							$values[$attrib][] = $value[$i];
						}
					}
				}

				// Create the 'LDIF'
				$dn						= "cn=" . $fqdn . "," . $USER_SEARCH_DN_CTR;
				$entry["objectClass"][] = "top";
				$entry["objectClass"][] = "qmailControl";
				$entry["cn"]			= $fqdn;
				foreach($values as $attrib => $val) {
					foreach($val as $value) {
						$entry[$attrib][] = $value;
					}
				}

				if(! ldap_add($_pql_control->ldap_linkid, $dn, $entry)) {
					unset($submit);

					$msg = urlencode("Failed to created mailserver $fqdn.");
					header("Location: <?=$PHP_SELF?>");
				} else {
					$msg = urlencode("Successfully created mailserver $fqdn.");
					header("Location: " . PQL_URI . "control_detail.php?host=$fqdn&msg=$msg&rlnb=1");
				}
			} else {
				$error_text["fqdn_clone"] = 'missing';
			}
		}
	}
?>
  <span class="title1">Create mailserver controls object in LDAP server <?=$USER_HOST_USR?></span>

  <table cellspacing="0" cellpadding="3" border="0">
    <th>
      <tr class="<?php table_bgcolor(); ?>">
        <td><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
        <td>You can either CLONE an existing server (<u>without</u> the locals/rcpthosts information)<br><b>or</b> CREATE a new server object.</td>
      </tr>
    </th>
  </table>

  <br><br>

  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Clone mailserver object</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Fully qualified domain name<br>of new mail server</td>
          <td><?php echo format_error($error_text["fqdn_clone"]); ?>
            <input type="text" name="fqdn" value="<?=$fqdn?>" size="30">
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Clone server</td>
          <td>
<?php
	$hosts = pql_control_get_hosts($_pql_control->ldap_linkid, $USER_SEARCH_DN_CTR);
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
      </th>
    </table>

    <input type="submit" name="submit" value="Clone">
  </form>

  <br>

  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Create mailserver object</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Fully qualified domain name</td>
          <td><?php echo format_error($error_text["fqdn_create"]); ?>
            <input type="text" name="fqdn" value="<?=$fqdn?>" size="30">
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Default domainname</td>
          <td><?php echo format_error($error_text["defaultdomain"]); ?>
              <input type="text" name="defaultdomain" value="<?=$defaultdomain?>" size="30">
          </td>
        </tr>
  
<?php
	if($ADVANCED_MODE) {
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Plusdomain</td>
          <td><input type="text" name="plusdomain" value="<?=$plusdomain?>" size="30">
        </tr>
  
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">LDAP Server</td>
          <td><?php echo format_error($error_text["ldapserver"]); ?>
              <input type="text" name="ldapserver" value="<?=$ldapserver?>" size="30">
          </td>
        </tr>
  
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">LDAP Search base</td>
          <td><?php echo format_error($error_text["ldapbasedn"]); ?>
              <input type="text" name="ldapbasedn" value="<?=$ldapbasedn?>" size="30">
          </td>
        </tr>
  
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Default quota</td>
          <td><input type="text" name="ldapdefaultquota" value="<?=$ldapdefaultquota?>" size="30">
        </tr>
  
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Default DOT mode</td>
          <td><input type="text" name="ldapdefaultdotmode" value="<?=$ldapdefaultdotmode?>" size="30">
        </tr>
  
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Directory maker</td>
          <td><input type="text" name="dirmaker" value="<?=$dirmaker?>" size="30">
        </tr>
  
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Default quota warning</td>
          <td><input type="text" name="quotawarning" value="<?=$quotawarning?>" size="30">
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
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
        <input type="hidden" name="plusdomain" value="">
        <input type="hidden" name="ldapserver" value="<?=$USER_HOST_USR?>">
        <input type="hidden" name="ldapbasedn" value="<?=$USER_HOST_CTR?>">
        <input type="hidden" name="ldapdefaultquota" value="">
        <input type="hidden" name="ldapdefaultdotmode" value="ldapwithprog">
        <input type="hidden" name="dirmaker" value="">
        <input type="hidden" name="quotawarning" value="User is above quota level!">
        <input type="hidden" name="ldaprebind" value="1">
<?php
	}
?>
      </th>
    </table>

    <input type="submit" name="submit" value="Create">
  </form>
<?php
}
// else - PQL_LDAP_CONTROL_USE not set

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
