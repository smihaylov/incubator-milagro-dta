// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

/*
Package api - service integration and contract types
*/
package api

import (
	"context"
	"net/http"

	"github.com/apache/incubator-milagro-dta/libs/logger"
	"github.com/apache/incubator-milagro-dta/libs/transport"
)

var (
	apiVersion = "v1"
)

//ClientService - enables service to be mocked
type ClientService interface {
	FulfillOrder(req *FulfillOrderRequest) (*FulfillOrderResponse, error)
	FulfillOrderSecret(req *FulfillOrderSecretRequest) (*FulfillOrderSecretResponse, error)
}

// MilagroClientService - implements Service Interface
type MilagroClientService struct {
	endpoints transport.ClientEndpoints
}

// ClientEndpoints return only the exported endpoints
func ClientEndpoints() transport.HTTPEndpoints {
	return transport.HTTPEndpoints{
		"FulfillOrder": {
			Path:        "/" + apiVersion + "/fulfill/order",
			Method:      http.MethodPost,
			NewRequest:  func() interface{} { return &FulfillOrderRequest{} },
			NewResponse: func() interface{} { return &FulfillOrderResponse{} },
		},
		"FulfillOrderSecret": {
			Path:        "/" + apiVersion + "/fulfill/order/secret",
			Method:      http.MethodPost,
			NewRequest:  func() interface{} { return &FulfillOrderSecretRequest{} },
			NewResponse: func() interface{} { return &FulfillOrderSecretResponse{} },
		},
	}
}

// NewHTTPClient returns Service backed by an HTTP server living at the remote instance
func NewHTTPClient(instance string, logger *logger.Logger) (ClientService, error) {
	clientEndpoints, err := transport.NewHTTPClient(instance, ClientEndpoints(), logger)
	return MilagroClientService{clientEndpoints}, err

}

//FulfillOrder -
func (c MilagroClientService) FulfillOrder(req *FulfillOrderRequest) (*FulfillOrderResponse, error) {
	endpoint := c.endpoints["FulfillOrder"]
	d, err := endpoint(context.Background(), req)
	if err != nil {
		return nil, err
	}
	r := d.(*FulfillOrderResponse)
	return r, nil
}

//FulfillOrderSecret -
func (c MilagroClientService) FulfillOrderSecret(req *FulfillOrderSecretRequest) (*FulfillOrderSecretResponse, error) {
	endpoint := c.endpoints["FulfillOrderSecret"]
	d, err := endpoint(context.Background(), req)
	if err != nil {
		return nil, err
	}
	r := d.(*FulfillOrderSecretResponse)
	return r, nil
}
