{* Invalid syntax error }
<span class="error">
LDAP Error #{$errno}: {$error}<p>
This error is caused because an attribute is used, but there is no 
<i>value</i> for it (i.e. an empty attribute/value pair).<br> The 
attribute is most likley <b>objectClass</b> and is usually caused 
because you're missing objectclasses for this type of object. Go to 
$url and setup the appropriate <b>objectclasses</b>.
</span>
