<?php /* $Id: user_add-details.inc,v 1.15 2004-03-26 11:23:16 turbo Exp $ */ ?>
<?php $objectclasses_included = pql_split_oldvalues(pql_get_define("PQL_CONF_OBJECTCLASS_USER", $_REQUEST["rootdn"])); ?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post" accept-charset="UTF-8">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('User data')?></th>
<?php if((!pql_get_define("PQL_CONF_AUTOCREATE_USERNAME", $_REQUEST["rootdn"]) and $_SESSION["ADVANCED_MODE"]) or !$_REQUEST["uid"]) {
?>
        <!-- Username (UID) -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Username')?></td>
          <td><?php echo pql_format_error_span($error_text["uid"]); ?><input type="text" name="uid" value="<?=$_REQUEST["uid"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?=$LANG->_('Numbers, letters and the following special chars: @, %, . (dot), _, -.\nIf left out, a username will be created automatically')?>.</td>
        </tr>

<?php } else { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?php echo pql_complete_constant($LANG->_('Automatically generated %what%'), array('what' => $LANG->_('username'))); ?> => <b><?=$_REQUEST["uid"]?></b>.</td>
        </tr>
        <input type="hidden" name="uid" value="<?=$_REQUEST["uid"]?>">

<?php } ?>
        <!-- Firstname -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Surname')?></td>
          <td>
              <?php echo pql_format_error_span($error_text["surname"]); ?>
              <input type="text" name="surname" value="<?=$_REQUEST["surname"]?>">
          </td>
        </tr>

        <!-- Lastname -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Lastname')?></td>
          <td>
            <?php echo pql_format_error_span($error_text["name"]); ?>
            <input type="text" name="name" value="<?=$_REQUEST["name"]?>">
          </td>
        </tr>
<?php if($_REQUEST["account_type"] != "forward") {
		// Get the default password scheme for branch
		$_REQUEST["defaultpasswordscheme"] = pql_domain_get_value($_pql, $_REQUEST["domain"],
																  pql_get_define("PQL_ATTR_DEFAULT_PASSWORDSCHEME"));

		if(!$_REQUEST["defaultpasswordscheme"] or $_SESSION["ADVANCED_MODE"])  {
			// We have no default password scheme - display forms for SYSTEM/MAIL account
?>

        <!-- Password scheme -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Password encryption scheme')?></td>
          <td>
<?php		if(eregi(',', pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["rootdn"]))) {
				// We got more than one password scheme...

				// Show each of the schemes as radio buttons
				$schemes = split(",", pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["rootdn"]));
				foreach($schemes as $scheme) {
?>
            <input type="radio" name="pwscheme" value="<?=$scheme?>" <?php if($_REQUEST["defaultpasswordscheme"] == $scheme) { echo "CHECKED"; } ?>><?=$scheme?>
<?php			}
			} else { ?>
            Scheme: <b>{<?php echo pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["rootdn"]); ?>}</b>
            <input type="hidden" name="pwscheme" value="<?php echo pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["rootdn"]); ?>">
<?php		} ?>
          </td>
        </tr>
<?php	} else { ?>
        <input type="hidden" name="pwscheme" value="<?=$_REQUEST["defaultpasswordscheme"]?>">
<?php	} ?>

<?php	if(eregi('KERBEROS', pql_get_define("PQL_CONF_PASSWORD_SCHEMES", $_REQUEST["rootdn"])) and (!$_REQUEST["defaultpasswordscheme"] or $_SESSION["ADVANCED_MODE"])) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?=$LANG->_('If using {KERBEROS} as password scheme, make sure you include the correct REALM (principal@REALM.TLD)')?></td>
        </tr>
<?php	} ?>

        <!-- Password -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Password')?></td>
          <!-- Crude hackery. Using type=password won't be so good if we're using {KERBEROS} -->
          <td>
            <?php echo pql_format_error_span($error_text["password"]); ?>
            <input type="input" name="password" value="<?=$_REQUEST["password"]?>">
            <input type="checkbox" name="crypted"><?=$LANG->_('Password is already encrypted')?>
            <?php echo pql_format_error_span($error["pwscheme"]); ?>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?=$LANG->_('If you enter an already encrypted password, you must make sure that the password scheme you\'ve choosen is the correct one. Also, choose the checkbox \uPassword is already encrypted\U')?></td>
        </tr>
<?php } ?>
      </th>
    </table>

    <br>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Account properties')?></th>
<?php if(($_REQUEST["account_type"] == "system") or ($_REQUEST["account_type"] == "shell")) {
		// display forms for SYSTEM account

		if($_SESSION["ADVANCED_MODE"]) {
			$shells = pql_get_valid_shells();
?>

        <!-- Loginshell -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Login shell')?></td>
          <td><?php echo pql_format_error_span($error["loginshell"]); ?>

            <select name="loginshell">
              <option value="/bin/false" SELECTED>/bin/false</option>
<?php
			foreach($shells as $shell) {
?>
              <option value="<?=$shell?>"><?=$shell?></option>
<?php
			}
?>
            </select>
          </td>
        </tr>
<?php
		} // end if ADVANCED mode
	} // end if account type == system
?>

<?php if($_REQUEST["account_type"] != "shell") {
		if(!pql_get_define("PQL_CONF_CREATE_ADDRESS", $_REQUEST["domain"]) or
		   (pql_get_define("PQL_CONF_CREATE_ADDRESS", $_REQUEST["domain"]) and !function_exists('user_generate_email')) or
		   $_SESSION["ADVANCED_MODE"] or $error_text["email"]) {
?>
        <!-- Email address -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Main address')?></td>
          <td>
            <?php echo pql_format_error_span($error_text["email"]); ?>
            <input type="text" name="email" value="<?=$_REQUEST["email"]?>">
<?php 		if(is_array($additionaldomainname)) { ?>
            <b>@ <select name="email_domain"></b>
              <option value="<?=$defaultdomain?>"><?=pql_maybe_idna_decode($defaultdomain)?></option>
<?php			foreach($additionaldomainname as $additional) { ?>
              <option value="<?=$additional?>"><?=pql_maybe_idna_decode($additional)?></option>
<?php   		} ?>
            </select>
<?php 		} else { ?>
            <b>@<?=pql_maybe_idna_decode($defaultdomain)?></b>
            <input type="hidden" name="email_domain" value="<?=$defaultdomain?>">
<?php 		} ?>
          </td>
        </tr>

<?php 		if(is_array($additionaldomainname)) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Alias')?></td>
          <td>
            <table>
              <td colspan="2"><input type="checkbox" name="include_additional" checked></td>
              <td>
                <?=$LANG->_('Include username in additional domains as alias')?>
              </td>
            </table>
          </td>
        </tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?=$LANG->_('The email address and the username will be automatically converted to lowercase')?></td>
        </tr>

        <tr></tr>
<?php 		}
		} else {
?>
        <tr class="<?php pql_format_table(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td><?php echo pql_complete_constant($LANG->_('Automatically generated %what%'), array('what' => $LANG->_('email address'))); ?>.</td>
        </tr>
<?php	}
	}
?>

        <!-- sub branch -->
<?php
	if($_SESSION["ADVANCED_MODE"]) {
		$branches = pql_unit_get($_pql->ldap_linkid, $_REQUEST["domain"]);
		if($branches[1]) {
			// More than one subbranch - show a select menu
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Put user in subbranch')?></td>
          <td>
            <select name="subbranch">
<?php
			for($i=0; $branches[$i]; $i++) {
?>
              <option value="<?=$branches[$i]?>"><?=$branches[$i]?></option>
<?php
			}
?>
            </select>
          </td>
        </tr>
<?php
		} else {
			// Only one subbranch
			if(pql_get_define("PQL_CONF_SUBTREE_USERS")) {
				$branch = pql_get_define("PQL_CONF_SUBTREE_USERS").",".$_REQUEST["domain"];
				$branch = pql_format_urls($branch);

				// Just to make sure - does this subbranch exists?!
				$xyz = pql_search($_pql->ldap_linkid, $branch, 'objectclass=*', 'dn', "BASE");
				if(is_array($xyz)) {
?>
        <input type="hidden" name="subbranch" value="<?=$branch?>">
<?php
				} else {
?>
        <input type="hidden" name="subbranch" value="<?=$url["domain"]?>">
<?php			}
			} else {
?>
        <input type="hidden" name="subbranch" value="<?=$url["domain"]?>">
<?php
			}
		}
	} else {
		if(pql_get_define("PQL_CONF_SUBTREE_USERS")) {
			$branch = pql_get_define("PQL_CONF_SUBTREE_USERS").",".$_REQUEST["domain"];
			$branch = pql_format_urls($branch);
			
			// Just to make sure - does this subbranch exists?!
			$xyz = pql_search($_pql->ldap_linkid, $branch, 'objectclass=*', 'dn', "BASE");
			if(is_array($xyz)) {
?>
        <input type="hidden" name="subbranch" value="<?=$branch?>">
<?php
			} else {
?>
        <input type="hidden" name="subbranch" value="<?=$url["domain"]?>">
<?php		}
		} else {
?>
        <input type="hidden" name="subbranch" value="<?=$url["domain"]?>">
<?php
		}
	} // end if ADVANCED mode

	if($_REQUEST["account_type"] == "forward") {
		// display forms for FORWARDING account
?>

        <!-- Forwarding address -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Forwarding address')?></td>
          <td>
            <?php echo pql_format_error_span($error_text["forwardingaddress"]); ?>
            <input type="text" name="forwardingaddress" value="<?=$_REQUEST["forwardingaddress"]?>"><?=$LANG->_('Email')?>
          </td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_('You can add more forwarding address in the user details page')?></td>
        </tr>
<?php
	} // account_type == forward
  
    if(($_REQUEST["account_type"] != 'shell') and in_array('trustaccount', $objectclasses_included)) {
?>
        <!-- Account status -->
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Status')?></td>
          <td>
            <select name="account_status">
              <option value="active" SELECTED><?=$LANG->_('Active')?></option>
              <option value="nopop"><?=$LANG->_('POP locked')?></option>
              <option value="disabled"><?=$LANG->_('Locked')?></option>
            </select>
          </td>
        </tr>
<?php } else { ?>
    <input type="hidden" name="account_status" value="">
<?php } ?>
      </th>
    </table>

    <input type="hidden" name="page_curr"    value="one">
<?php
	if(($_SESSION["ADVANCED_MODE"] == 0) or ($_REQUEST["account_type"] == "forward")) {
		// Go to save, no next form...
		if($_SESSION["ADVANCED_MODE"] == 0) {
			// Autocreate some values by using 'safe' (?) defaults
?>
    <input type="hidden" name="loginshell"    value="/bin/false">
    <input type="hidden" name="homedirectory" value="">
    <input type="hidden" name="maildirectory" value="">
    <input type="hidden" name="host"          value="default">
<?php	} ?>
    <input type="hidden" name="page_next"     value="save">
<?php
	} else {
		if($_REQUEST["account_type"] != 'shell') {
?>
    <input type="hidden" name="page_next"     value="two">
<?php
		} else {
?>
    <input type="hidden" name="page_next"     value="save">
<?php
		}
	} // account_type == forward
?>
    <input type="hidden" name="rootdn"        value="<?=$url["rootdn"]?>">
    <input type="hidden" name="domain"        value="<?=$url["domain"]?>">
    <input type="hidden" name="account_type"  value="<?=$_REQUEST["account_type"]?>">

    <br>

<?php
	if($_SESSION["ADVANCED_MODE"] == 1) {
?>
    <input type="submit" value="--&gt;&gt;">
<?php
	} else {
?>
    <input type="submit" value="<?=$LANG->_('Save')?>">
<?php
	}
?>
  </form>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
