<?php
// Show details on QmailLDAP/Control host
// $Id: control_detail.php,v 1.15 2003-05-04 10:04:57 turbo Exp $
session_start();
require("./include/pql_config.inc");

if($config["PQL_GLOB_CONTROL_USE"]) {
    // include control api if control is used
    include("./include/pql_control.inc");
    $_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);

	include("./header.html");

	// Get the values of the mailserver
	$attribs = array("defaultdomain", "plusdomain", "ldapserver",
					 "ldaprebind", "ldapbasedn", "ldapdefaultquota",
					 "ldapdefaultdotmode", "dirmaker", "quotawarning",
					 "locals", "rcpthosts", "ldaplogin", "ldappassword");
	$cn = "cn=" . $mxhost . "," . $USER_SEARCH_DN_CTR;

	foreach($attribs as $attrib) {
		$value = pql_control_get_attribute($_pql_control->ldap_linkid, $cn, $attrib);
		if(!is_null($value)) {
			if($attrib == "locals") {
				foreach($value as $val) {
					$locals[] = $val;
				}
			} elseif($attrib == "rcpthosts") {
				foreach($value as $val) {
					$rcpthosts[] = $val;
				}
			} elseif($attrib == "ldappassword") {
				$$attrib = "encrypted";
			} else {
				$$attrib = $value[0];
			}
		} else {
			if($attrib == 'ldapserver')
			  $$attrib = "<i>".$USER_HOST."</i>";
			elseif($attrib == 'ldapbasedn')
			  $$attrib = "<i>".$USER_SEARCH_DN_CTR."</i>";
			else
			  $$attrib = "<i>not set</i>";
		}
	}

	// print status message, if one is available
	if(isset($msg)){
		print_status_msg($msg);
	}

	// reload navigation bar if needed
	if(isset($rlnb) and $config["PQL_GLOB_AUTO_RELOAD"]) {
?>

  <script src="frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
	parent.frames.pqlnavctrl.location.reload();
  //--></script>
<?php
	}
?>

  <span class="title1">Mailserver: <?=$mxhost?></span>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Base values</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">RDN</td>
        <td><?=$cn?></td>
        <td></td>
        <td></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Fully qualified domain name</td>
        <td><?=$mxhost?></td>
        <td></td>
        <td></td>
      </tr>
  
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Default domainname</td>
        <td><?=$defaultdomain?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=defaultdomain&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=defaultdomain&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Plusdomain</td>
        <td><?=$plusdomain?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=plusdomain&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=plusdomain&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">LDAP Server</td>
        <td><?=$ldapserver?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=ldapserver&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=ldapserver&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">LDAP Search base</td>
        <td><?=$ldapbasedn?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=ldapbasedn&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=ldapbasedn&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">LDAP login</td>
        <td><?=$ldaplogin?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=ldaplogin&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=ldaplogin&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">LDAP password</td>
        <td><?=$ldappassword?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=ldappassword&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=ldappassword&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">LDAP Rebind</td>
        <td><?php if($ldaprebind) { echo "Yes"; $set=0; } else { echo "No"; $set=1; } ?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=ldaprebind&submit=1&type=modify&set=<?=$set?>"><img src="images/edit.png" width="12" height="12" alt="toggle value" border="0"></a></td>
        <td></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Default quota</td>
        <td><?=$ldapdefaultquota?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=ldapdefaultquota&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=ldapdefaultquota&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Default DOT mode</td>
        <td><?=$ldapdefaultdotmode?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=ldapdefaultdotmode&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=ldapdefaultdotmode&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

<?php
	if($dirmaker) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Directory maker</td>
        <td><?=$dirmaker?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=dirmaker&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=dirmaker&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

<?php
	}

	if($quotawarning) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Default quota warning</td>
        <td><?=$quotawarning?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=quotawarning&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=quotawarning&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

<?php
	}
?>
    </th>
  </table>

  <br>

  <table cellspacing="0" cellpadding="0" border="0">
    <th valign="top" align="left">Locals
      <!-- LOCALS -->
      <table cellspacing="0" cellpadding="3" border="0">
        <th>
          <tr class="<?php table_bgcolor(); ?>">
            <td class="title">locals</td>
<?php
	$i = 0;
	if(is_array($locals)) {
		foreach($locals as $local) {
			if(!$i) {
?>
            <td><?=$local?></td>
            <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=locals&type=add&set=<?=$local?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=locals&type=del&set=<?=$local?>&submit=1"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
          </tr>

<?php
			} else {
?>
          <tr class="<?php table_bgcolor(); ?>">
            <td class="title"></td>
            <td><?=$local?></td>
            <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=locals&type=add&set=<?=$local?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=locals&type=del&set=<?=$local?>&submit=1"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
          </tr>

<?php
			}
			$i++;
		}
	} else {
?>
          <td><i>not defined</i></td>
          <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=locals&type=add&set=<?=$local?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=locals&type=del&set=<?=$local?>&submit=1"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
        </tr>
<?php
	}
?>
          <tr class="<?php table_bgcolor(); ?>">
            <td class="title" align="right"><img src="images/edit.png" width="12" height="12"></td>
            <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=locals">Add value</a></td>
            <td></td>
          </tr>

          <tr class="<?php table_bgcolor(); ?>">
            <td class="title" align="right"><img src="images/edit.png" width="12" height="12"></td>
            <td><a href="control_edit_attribute.php?attrib=locals&type=replicate&mxhost=<?=$mxhost?>&submit=1">Replicate</a></td>
            <td></td>
          </tr>
        </th>
      </table>
    </th>

    <th>&nbsp;&nbsp;&nbsp;&nbsp;</th>

    <th valign="top" align="left">RCPT Hosts
      <!-- RCPTHOSTS -->
      <table cellspacing="0" cellpadding="3" border="0">
        <th>
          <tr class="<?php table_bgcolor(); ?>">
            <td class="title">rcpthosts</td>
<?php
	$i = 0;
	if(is_array($rcpthosts)) {
		foreach($rcpthosts as $rcpthost) {
			if(!$i) {
?>
            <td><?=$rcpthost?></td>
            <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=rcpthosts&type=add&set=<?=$rcpthost?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=rcpthosts&type=del&set=<?=$rcpthost?>&submit=1"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
          </tr>

<?php
			} else {
?>
          <tr class="<?php table_bgcolor(); ?>">
            <td class="title"></td>
            <td><?=$rcpthost?></td>
            <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=rcpthosts&type=add&set=<?=$rcpthost?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=rcpthosts&type=del&set=<?=$rcpthost?>&submit=1"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
          </tr>

<?php
			}
			$i++;
		}
	} else {
?>
          <td><i>not defined</i></td>
          <td></td>
        </tr>
<?php
	}
?>
          <tr class="<?php table_bgcolor(); ?>">
            <td class="title" align="right"><img src="images/edit.png" width="12" height="12"></td>
            <td><a href="control_edit_attribute.php?mxhost=<?=$mxhost?>&attrib=rcpthosts">Add value</a></td>
            <td></td>
          </tr>
        </th>
      </table>
    </th>
  </table>

  <br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=PQL_LANG_ACTIONS?></th>
      <tr class="subtitle">
        <td colspan="4"><a href="installmailserver.php?domain=<?=$domain?>&mxhost=<?=$mxhost?>"><img src="images/edit.png" width="12" height="12" border="0">Create mailserver install script</a></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td><a href="control_del.php?mxhost=<?=$mxhost?>"><img src="images/edit.png" width="12" height="12" border="0">Delete mailserver control object</a></td>
      </tr>
    </th>
  </table>
<?php  
} else {
	// PQL_GLOB_CONTROL_USE isn't set!
?>
  <span class="title1">PQL_GLOB_CONTROL_USE isn't set, won't show control information</span>
<?php
}

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
