<?php
// Add a ezmlm mailinglist
// $Id: ezmlm_add.php,v 1.2 2002-12-22 16:32:50 turbo Exp $
//
session_start();

require("include/pql.inc");
$_pql = new pql($USER_HOST_USR, $USER_DN, $USER_PASS);

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
if($moderated)		$checked["moderated"]		= " CHECKED";
if($submoderated)	$checked["submoderated"]	= " CHECKED";
if($prefix)			$checked["prefix"]			= " CHECKED";
if($digest)			$checked["digest"]			= " CHECKED";
if($remote)			$checked["remote"]			= " CHECKED";
if($indexed)		$checked["indexed"]			= " CHECKED";
if($sublistable)	$checked["sublistable"]		= " CHECKED";
if($reqaddress)		$checked["reqaddress"]		= " CHECKED";
if($trailers)		$checked["trailers"]		= " CHECKED";
if($subonly)		$checked["subonly"]			= " CHECKED";
if($emptysubjects)	$checked["emptysubjects"]	= " CHECKED";
if($extras)			$checked["extras"]			= " CHECKED";
if($archived)		$checked["archived"]		= " CHECKED";
if(!$archived) {
	$checked["guardedarchive"] = " DISABLED";
} else {
	if($guardedarchive)
	  $checked["guardedarchive"] = " CHECKED";
}
if($pubpriv == 'public') {
	$checked["public"]  = " CHECKED";
	$checked["private"] = "";
} elseif($pubpriv == 'private') {
	$checked["public"]  = "";
	$checked["private"] = " CHECKED";
}

// Create the mailinglist creation form
include("header.html");

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

  <form action="<?=$PHP_SELF?>" method="post">
    <!-- Base configuration -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Base configuration</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">List name</td>
          <td>
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
          <td><input name="listowner" value="<?=$listowner?>"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Parent list</td>
          <td><input name="listparent" value="<?=$listparent?>"></td>
        </tr>
      </th>
    </table>

    <br>

    <!-- List options -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th valign="top" align="left">Basic List options
        <table cellspacing="0" cellpadding="3" border="0">
          <th>
            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="moderated" accesskey="m" onChange="this.form.submit()"<?=$checked["moderated"]?>></td>
              <td class="title">Moderation - <u>M</u>essage</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="submoderated" accesskey="s" onChange="this.form.submit()"<?=$checked["submoderated"]?>></td>
              <td class="title">Moderation - <u>S</u>ubscription</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="prefix" accesskey="f" onChange="this.form.submit()"<?=$checked["prefix"]?>></td>
              <td class="title">Subject pre<u>f</u>ix</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="digest" accesskey="d" onChange="this.form.submit()"<?=$checked["digest"]?>></td>
              <td class="title">Enable <u>d</u>igest</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="remote" accesskey="c" onChange="this.form.submit()"<?=$checked["remote"]?>></td>
              <td class="title">Enable remote <u>c</u>onfiguration</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="archived" accesskey="a" onChange="this.form.submit()"<?=$checked["archived"]?>></td>
              <td class="title"><u>A</u>rchived</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="guardedarchive" accesskey="g" onChange="this.form.submit()"<?=$checked["guardedarchive"]?>></td>
              <td class="title"><u>G</u>uarded archive</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="indexed" accesskey="i" onChange="this.form.submit()"<?=$checked["indexed"]?>></td>
              <td class="title"><u>I</u>ndexed</td>
            </tr>
          </th>
        </table>
      </th>

      <th valign="top" align="left">Extra List options
        <table cellspacing="0" cellpadding="3" border="0">
          <th>
            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="sublistable" accesskey="l" onChange="this.form.submit()"<?=$checked["sublistable"]?>></td>
              <td class="title">Subscriber <u>l</u>istable</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="reqaddress" accesskey="q" onChange="this.form.submit()"<?=$checked["reqaddress"]?>></td>
              <td class="title">Enable re<u>q</u>uest address</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="trailers" accesskey="t" onChange="this.form.submit()"<?=$checked["trailers"]?>></td>
              <td class="title">Add <u>t</u>railers to messages</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="subonly" accesskey="u" onChange="this.form.submit()"<?=$checked["trailers"]?>></td>
              <td class="title">Allow only s<u>u</u>bscribers posts</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="emptysubjects" accesskey="a" onChange="this.form.submit()"<?=$checked["emptysubjects"]?>></td>
              <td class="title"><u>A</u>ccept empty subjects</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="extras" accesskey="x" onChange="this.form.submit()"<?=$checked["extras"]?>></td>
              <td class="title">Enable list e<u>x</u>tras</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="radio" name="pubpriv" accesskey="P" onChange="this.form.submit()" value="public"<?=$checked["public"]?>></td>
              <td class="title"><u>P</u>ublic</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="radio" name="pubpriv" accesskey="p" onChange="this.form.submit()" value="private"<?=$checked["private"]?>></td>
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
          <td><input type="text" name="subscriber[<?=$i?>]" value="<?=$subscriber[$i]?>" onChange="this.form.submit()"></td>
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
          <td><input type="text" name="killlist[<?=$i?>]" value="<?=$killlist[$i]?>" onChange="this.form.submit()"></td>
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
    <input type="submit" value="create list">
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
