#ifndef __server_config_h__
#define __server_config_h__ 1
/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */


/*
 * Configuration record for listener and connector entities
 */

#include "qpid/dispatch/error.h"
#include "qpid/dispatch/failoverlist.h"

#include <stddef.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct pn_data_t     pn_data_t;
typedef struct qd_entity_t   qd_entity_t;
typedef struct qd_dispatch_t qd_dispatch_t;


/**
 * Configuration block for a connector or a listener.
 */
typedef struct qd_server_config_t {
    /**
     * Host name or network address to bind to a listener or use in the connector.
     */
    char *host;

    /**
     * Port name or number to bind to a listener or use in the connector.
     */
    char *port;

    /**
     * Socket address family that the socket will use when binding listener or connector.
     * Possible values are IPv4 or IPv6. If not specified, the protocol family will be automatically determined from the address
     */
    char *socket_address_family;

    /**
     * Expose simple liveness check.
     */
    bool healthz;

    /**
     * Export metrics.
     */
    bool metrics;

    /**
     * Websockets enabled.
     */
    bool websockets;

    /**
     * Accept HTTP connections, allow WebSocket "amqp" protocol upgrades.
     */
    bool http;

    /**
     * Directory for HTTP content
     */
    char *http_root_dir;

    /**
     * Connection name, used as a reference from other parts of the configuration.
     */
    char *name;

    /**
     * Space-separated list of SASL mechanisms to be accepted for the connection.
     */
    char *sasl_mechanisms;

    /**
     * If appropriate for the mechanism, the username for authentication
     * (connector only)
     */
    char *sasl_username;

    /**
     * If appropriate for the mechanism, the password for authentication
     * (connector only)
     */
    char *sasl_password;

    /**
     * If appropriate for the mechanism, the minimum acceptable security strength factor
     */
    int sasl_minssf;

    /**
     * If appropriate for the mechanism, the maximum acceptable security strength factor
     */
    int sasl_maxssf;

    /**
     * Iff true, SSL/TLS must be used on the connection.
     */
    bool ssl_required;

    /**
     * Iff true, the client of the connection must authenticate with the server.
     */
    bool requireAuthentication;

    /**
     * Iff true, client authentication _may_ be insecure (i.e. PLAIN over plaintext).
     */
    bool allowInsecureAuthentication;

    /**
     * Iff true, the payload of the connection must be encrypted.
     */
    bool requireEncryption;

    /**
     * Ensures that when initiating a connection (as a client) the host name in the URL to which this connector
     * connects to matches the host name in the digital certificate that the peer sends back as part of the SSL connection
     */
    bool verify_host_name;

    /**
     * If true, strip the inbound qpid dispatch specific message annotations. This only applies to ingress and egress routers.
     * Annotations generated by inter-router messages will be untouched.
     */
    bool strip_inbound_annotations;

    /**
     * If true, strip the outbound qpid dispatch specific message annotations. This only applies to ingress and egress routers.
     * Annotations generated by inter-router messages will be untouched.
     */
    bool strip_outbound_annotations;

    /**
     * The number of deliveries that can be in-flight concurrently for each link within the connection.
     */
    int link_capacity;

    /**
     * Path to the file containing the PEM-formatted public certificate for the local end
     * of the connection.
     */
    char *ssl_certificate_file;

    /**
     * Path to the file containing the PEM-formatted private key for the local end of the
     * connection.
     */
    char *ssl_private_key_file;

    /**
     * Holds the list of component fields of the client certificate from which a unique identifier is constructed.
     * For e.g, this field could have the format of 'cou' indicating that the uid will consist of
     * c - common name concatenated with o - organization-company name concatenated with u - organization unit
     * Allowed components are
     * Allowed values can be any combination of comma separated
     * 'c'( ISO3166 two character country code),
     * 's'(state or province),
     * 'l'(Locality; generally - city),
     * 'o'(Organization - Company Name),
     * 'u'(Organization Unit - typically certificate type or brand),
     * 'n'(CommonName - typically a user name for client certificates) and
     * '1'(sha1 certificate fingerprint, the fingerprint, as displayed in the fingerprints section when looking at a certificate
     *  with say a web browser is the hash of the entire certificate in DER form)
     * '2'(sha256 certificate fingerprint)
     * '5'(sha512 certificate fingerprint)
     */
    char *ssl_uid_format;

    /**
     * The name of the related ssl profile.
     */
    char *ssl_profile;

    /**
     * Full path to the file that contains the uid to display name mapping.
     */
    char *ssl_uid_name_mapping_file;

    /**
     * The password used to sign the private key, or NULL if the key is not protected.
     */
    char *ssl_password;

    /**
     * Path to the file containing the PEM-formatted set of certificates of trusted CAs.
     */
    char *ssl_trusted_certificate_db;

    /**
     * Iff true, require that the peer's certificate be supplied and that it be authentic
     * according to the set of trusted CAs.
     */
    bool ssl_require_peer_authentication;

    /**
     * Specifies the enabled ciphers so the SSL Ciphers can be hardened.
     */
    char *ssl_ciphers;

    /**
     * This list is a space separated string of the allowed TLS protocols. The current possibilities are TLSv1 TLSv1.1 TLSv1.2.
     * For example, if you want to permit only TLSv.1.1 and TLSv1.2, your value for the protocols would be TLSv1.1 TLSv1.2. If this attribute is not set, then all the TLS protocols are allowed.
     */
    char *ssl_protocols;

    /**
     * Allow the connection to be redirected by the peer (via CLOSE->Redirect).  This is
     * meaningful for outgoing (connector) connections only.
     */
    bool allow_redirect;

    /**
     * MultiTenancy support.  If true, the vhost is used to define the address space of
     * addresses used over this connection.
     */
    bool multi_tenant;

    /**
     * Optional vhost to use for policy lookup.  If non-null, this overrides the vhost supplied
     * in the OPEN from the peer only for the purpose of identifying the policy to enforce.
     */
    char *policy_vhost;

    /**
     * The specified role of the connection.  This can be used to control the behavior and
     * capabilities of the connections.
     */
    char *role;

    /**
     * If the role is "inter-router", the cost can be set to a number greater than
     * or equal to one.  Inter-router cost is used to influence the routing algorithm
     * such that it prefers lower-cost paths.
     */
    int inter_router_cost;

    /**
     * The maximum size of an AMQP frame in octets.
     */
    uint32_t max_frame_size;

    /**
     * The max_sessions value is the number of sessions allowed on the Connection. 
     */
    uint32_t max_sessions;

    /**
     * The incoming capacity value is calculated to be (sessionMaxFrames * maxFrameSize).
     * In a round about way the calculation forces the AMQP Begin/incoming-capacity value
     * to equal the specified sessionMaxFrames value measured in units of transfer frames.
     * This calculation is done to satisfy proton pn_session_set_incoming_capacity().
     */
    size_t incoming_capacity;

    /**
     * The idle timeout, in seconds.  If the peer sends no data frames in this many seconds, the
     * connection will be automatically closed.
     */
    int idle_timeout_seconds;

    /**
     * The timeout, in seconds, for the initial connection handshake.  If a connection is established
     * inbound (via a listener) and the timeout expires before the OPEN frame arrives, the connection
     * shall be closed.
     */
    int initial_handshake_timeout_seconds;

    /**
     *  Holds comma separated list that indicates which components of the message should be logged.
     *  Defaults to 'none' (log nothing). If you want all properties and application properties of the message logged use 'all'.
     *  Specific components of the message can be logged by indicating the components via a comma separated list.
     *  The components are
     *  message-id
     *   user-id
     *   to
     *   subject
     *   reply-to
     *   correlation-id
     *   content-type
     *   content-encoding
     *   absolute-expiry-time
     *   creation-time
     *   group-id
     *   group-sequence
     *   reply-to-group-id
     *   app-properties.
     */
    char *log_message;

    /**
     * A bitwise representation of which log components have been enabled in the log_message field.
     */
    uint32_t message_log_flags;

    /**
     * Configured failover list
     */
    qd_failover_list_t *failover_list;

    /**
     * Extra connection properties to include in the outgoing Open frame.  Stored as a map.
     */
    pn_data_t *conn_props;

    /**
     * For inter-router roles only.  The number of data connections associated with the link.
     */
    char *data_connection_count;
    bool  has_data_connectors;

    /**
     * @name These fields are not primary configuration, they are computed.
     * @{
     */

    /**
     * Concatenated connect/listen address "host:port"
     */
    char *host_port;

    /**
     * @}
     */
} qd_server_config_t;


qd_error_t qd_server_config_load(qd_dispatch_t *qd, qd_server_config_t *cf, qd_entity_t *entity, bool is_listener, const char *role_override);
void qd_server_config_free(qd_server_config_t *cf);
void qd_server_config_process_password(char **actual_val, char *pw, bool *is_file, bool allow_literal_prefix);
void qd_set_password_from_file(const char *password_file, char **password_field);

#endif
