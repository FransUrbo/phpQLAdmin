<?php
// Add a ezmlm mailinglist
// $Id: ezmlm_add.php,v 1.4 2002-12-23 19:51:35 turbo Exp $
//
session_start();

require("include/pql.inc");
$_pql = new pql($USER_HOST_USR, $USER_DN, $USER_PASS);

// Initialize
require("include/pql_ezmlm.inc");
$ezmlm = new ezmlm();
require("ezmlm-hardcoded.php");

if(!$subscribercount) {
	$subscribercount = 0;
}
if(!$killcount) {
	$killcount = 0;
}

if(!$domain) {
	// Get list of domain
	$domains = pql_get_domain_value($_pql->ldap_linkid, '*', 'administrator', "=" . $USER_DN);
    if(!is_array($domains)){
		// if no domain defined, report it
		echo "<b>Can't find any domain!</b><br>";
    } else {
		asort($domains);
		
		if(is_array($domains)){
			// Get the domainname from the domain object
			foreach($domains as $key => $d) {
				$dont_add = 0;
				
				// Get the default domainname for the domain
				$domainname = pql_get_domain_value($_pql->ldap_linkid, $d, "defaultdomain");
				
				// Remove duplicates
				if($domain_list) {
					for($i=0; $domain_list[$i]; $i++) {
						if($domain_list[$i] == $domainname) {
							$dont_add = 1;
						}
					}
				}
				
				if(!$dont_add) {
					$domain_list[] = $domainname;
				}
			}
		}
	}
}

// Remember values between reloads
if($archived)		$checked["archived"]		= " CHECKED";	// -aA
if($remotecfg)		$checked["remotecfg"]		= " CHECKED";	// -cC
if($digest)			$checked["digest"]			= " CHECKED";	// -dD
if($prefix)			$checked["prefix"]			= " CHECKED";	// -fF
if($subhelp)		$checked["subhelp"]			= " CHECKED";	// -hH
if(!$archived) {	$guardarch = " DISABLED";	}
if($indexed)		$checked["indexed"]			= " CHECKED";	// -iI
if($subjump)		$checked["subjump"]			= " CHECKED";	// -jJ
if($sublistable)	$checked["sublistable"]		= " CHECKED";	// -lL
																// -kK TODO
if($moderated)		$checked["moderated"]		= " CHECKED";	// -mM
if($modonly)		$checked["modonly"]			= " CHECKED";	// -oO
if($pubpriv == 'public') {
	$checked["public"]  = " CHECKED";							// -p
	$checked["private"] = "";
} elseif($pubpriv == 'private') {
	$checked["public"]  = "";
	$checked["private"] = " CHECKED";							// -P
}
if($reqaddress)		$checked["reqaddress"]		= " CHECKED";	// -qQ
if($remoteadm)		$checked["remoteadm"]		= " CHECKED";	// -rR
if($submoderated)	$checked["submoderated"]	= " CHECKED";	// -sS
if($trailers)		$checked["trailers"]		= " CHECKED";	// -tT
if($subonly)		$checked["subonly"]			= " CHECKED";	// -uU
if($extras)			$checked["extras"]			= " CHECKED";	// -x

// TODO: For some reason, when I enable this, I get errors on the page!
//$onchg = ' onChange="this.form.submit()"';

// Create list
if(isset($submit)) {
	if($listname)
	  $ezmlm->updatelistentry(1, $listname, $domain, $checked);
	else
	  $error_text["listname"] = 'missing';
}

require("header.html");

if(!$domain) {
?>
  <span class="title1">Create mailinglist</span>
<?php
} else {
?>
  <span class="title1">Create mailinglist in domain <?=$domain?></span>
<?php
}
?>
  <br><br>

  <form action="<?=$PHP_SELF?>" method="post" name"addlist">
    <!-- Base configuration -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Base configuration</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">List name</td>
          <td><?php echo format_error($error_text["listname"]); ?>

<?php
if(!$domain) {
	// No domain - select box with existing domains
?>
            <input name="listname" value="<?=$listname?>">@
            <select name="domain">
<?php
	for($i=0; $domain_list[$i]; $i++) {
?>
	          <option value="<?=$domain_list[$i]?>"><?=$domain_list[$i]?></option>
<?php
	}
?>
            </select>
<?php
} else {
	// Got domainname, show that (and remember it!)
?>
            <input type="hidden" name="domain" value="<?=$domain?>">
            <input name="listname" value="<?=$listname?>"><b>@<?=$domain?></b>
<?php
}
?>
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">List owner</td>
          <td>
            <input name="listowner" value="<?=$listowner?>">
            <i>(<b>optional</b>: if not mailbox in list directory)</i>
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">From address</td>
          <td>
            <input name="fromaddress" value="<?=$fromaddress?>">
            <i>(<b>optional</b>: if not same as listname)</i>
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Parent list</td>
          <td>
            <input name="listparent" value="<?=$listparent?>">
            <i>(<b>optional</b>: Make the list a sublist of list)</i>
          </td>
        </tr>
      </th>
    </table>

    <br>

    <!-- List options -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th valign="top" align="left">Subscription options
        <table cellspacing="0" cellpadding="3" border="0">
          <th>
            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="subhelp" accesskey="h"<?=$onchg?><?=$checked["subhelp"]?>></td>
              <td class="title">Subscription verification</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="subjump" accesskey="j"<?=$onchg?><?=$checked["subjump"]?>></td>
              <td class="title">Unsubscription verification</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="submoderated" accesskey="s"<?=$onchg?><?=$checked["submoderated"]?>></td>
              <td class="title"><u>S</u>ubscription moderation</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="sublistable" accesskey="l"<?=$onchg?><?=$checked["sublistable"]?>></td>
              <td class="title">Subscriber <u>l</u>istable</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="reqaddress" accesskey="q"<?=$onchg?><?=$checked["reqaddress"]?>></td>
              <td class="title">Enable re<u>q</u>uest address</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="subonly" accesskey="u"<?=$onchg?><?=$checked["trailers"]?>></td>
              <td class="title">Allow only s<u>u</u>bscribers posts</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="modonly" accesskey="o"<?=$onchg?><?=$checked["modonly"]?>></td>
              <td class="title">Allow only m<u>o</u>derator posts</td>
            </tr>
          </th>
        </table>
      </th>

      <th valign="top" align="left">Basic List options
        <table cellspacing="0" cellpadding="3" border="0">
          <th>
            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="moderated" accesskey="m"<?=$onchg?><?=$checked["moderated"]?>></td>
              <td class="title">Moderation - <u>M</u>essage</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="prefix" accesskey="f"<?=$onchg?><?=$checked["prefix"]?>></td>
              <td class="title">Subject pre<u>f</u>ix</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="remotecfg" accesskey="c"<?=$onchg?><?=$checked["remotecfg"]?>></td>
              <td class="title">Enable remote <u>c</u>onfiguration</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="remoteadm" accesskey="r"<?=$onchg?><?=$checked["remoteadm"]?>></td>
              <td class="title">Enable <u>r</u>emote administration</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="archived" accesskey="a"<?=$onchg?><?=$checked["archived"]?>></td>
              <td class="title"><u>A</u>rchived</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="guardedarchive" accesskey="g"<?=$onchg?><?=$guardarch?>></td>
              <td class="title"><u>G</u>uarded archive</td>
            </tr>
          </th>
        </table>
      </th>

      <th valign="top" align="left">Extra List options
        <table cellspacing="0" cellpadding="3" border="0">
          <th>
            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="digest" accesskey="d"<?=$onchg?><?=$checked["digest"]?>></td>
              <td class="title">Enable <u>d</u>igest</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="indexed" accesskey="i"<?=$onchg?><?=$checked["indexed"]?>></td>
              <td class="title"><u>I</u>ndexed</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="trailers" accesskey="t"<?=$onchg?><?=$checked["trailers"]?>></td>
              <td class="title">Add <u>t</u>railers to messages</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="extras" accesskey="x"<?=$onchg?><?=$checked["extras"]?>></td>
              <td class="title">Enable list e<u>x</u>tras</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="radio" name="pubpriv" accesskey="p"<?=$onchg?> value="public"<?=$checked["public"]?>></td>
              <td class="title"><u>P</u>ublic</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="radio" name="pubpriv" accesskey="p"<?=$onchg?> value="private"<?=$checked["private"]?>></td>
              <td class="title"><u>P</u>rivate</td>
            </tr>
          </th>
        </table>
      </th>
    </table>

    <br><!-- ======================================================= -->

    <!-- add subscribers at creation -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Subscriber address(es)</th>
<?php
	for($i = 1; $i <= $subscribercount; $i++) {
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Subscriber</td>
          <td><input type="text" name="subscriber[<?=$i?>]" value="<?=$subscriber[$i]?>"<?=$onchg?>></td>
        </tr>
<?php
	}
?>
        <tr class="subtitle">
          <td><a href="<?php echo $PHP_SELF; ?>?subscribercount=<?php echo ($subscribercount + 1); ?>">add <?php if($subscribercount){echo 'additional ';}?>address</a></td>
        </tr>
      </th>
    </table>
    <!-- add subscribers at creation -->

    <br><!-- ======================================================= -->

    <!-- add kill at creation -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Rejected address(es)</th>
<?php
	for($i = 1; $i <= $killcount; $i++) {
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Address</td>
          <td><input type="text" name="killlist[<?=$i?>]" value="<?=$killlist[$i]?>"<?=$onchg?>></td>
        </tr>

<?php
	}
?>
        <tr class="subtitle">
          <td><a href="<?php echo $PHP_SELF; ?>?killcount=<?php echo ($killcount + 1); ?>">add <?php if($killcount){echo 'additional ';}?>address</a></td>
        </tr>
      </th>
    </table>
    <!-- add kill at creation -->

    <br><!-- ======================================================= -->

<?php for($i = 1; $i <= $subscribercount; $i++) { ?>
    <input type="hidden" name="subscriber[<?=$i?>]" value="<?=$subscriber[$i]?>">
<?php } for($i = 1; $i <= $killcount; $i++) { ?>
    <input type="hidden" name="killlist[<?=$i?>]" value="<?=$killlist[$i]?>">
<?php } ?>

    <input type="submit" name="submit" value="Create list">
  </form>
  </body>
</html>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
