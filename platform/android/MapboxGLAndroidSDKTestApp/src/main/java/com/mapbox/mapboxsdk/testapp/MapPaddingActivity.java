package com.mapbox.mapboxsdk.testapp;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;

import com.mapbox.mapboxsdk.annotations.MarkerOptions;
import com.mapbox.mapboxsdk.camera.CameraPosition;
import com.mapbox.mapboxsdk.constants.MyLocationTracking;
import com.mapbox.mapboxsdk.constants.Style;
import com.mapbox.mapboxsdk.geometry.LatLng;
import com.mapbox.mapboxsdk.maps.CameraUpdateFactory;
import com.mapbox.mapboxsdk.maps.MapView;
import com.mapbox.mapboxsdk.maps.MapboxMap;
import com.mapbox.mapboxsdk.maps.OnMapReadyCallback;
import com.mapbox.mapboxsdk.maps.TrackingSettings;
import com.mapbox.mapboxsdk.utils.ApiAccess;

public class MapPaddingActivity extends AppCompatActivity {

    private MapView mMapView;
    private MapboxMap mMapboxMap;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_map_padding);

        final Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setDisplayHomeAsUpEnabled(true);
            actionBar.setDisplayShowHomeEnabled(true);
        }

        mMapView = (MapView) findViewById(R.id.mapView);
        mMapView.setTag(true);
        mMapView.setAccessToken(ApiAccess.getToken(this));
        mMapView.onCreate(savedInstanceState);
        mMapView.getMapAsync(new OnMapReadyCallback() {
            @Override
            public void onMapReady(@NonNull MapboxMap mapboxMap) {
                mMapboxMap = mapboxMap;
                mapboxMap.setStyle(Style.MAPBOX_STREETS);

                int paddingLeft = (int) getResources().getDimension(R.dimen.map_padding_left);
                int paddingBottom = (int) getResources().getDimension(R.dimen.map_padding_bottom);
                int paddingRight = (int) getResources().getDimension(R.dimen.map_padding_right);
                mapboxMap.setPadding(paddingLeft, toolbar.getHeight(), paddingRight, paddingBottom);
            }
        });
    }

    @Override
    protected void onStart() {
        super.onStart();
        mMapView.onStart();
    }

    @Override
    public void onResume() {
        super.onResume();
        mMapView.onResume();
    }

    @Override
    public void onPause() {
        super.onPause();
        mMapView.onPause();
    }

    @Override
    protected void onStop() {
        super.onStop();
        mMapView.onStop();
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        mMapView.onSaveInstanceState(outState);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mMapView.onDestroy();
    }

    @Override
    public void onLowMemory() {
        super.onLowMemory();
        mMapView.onLowMemory();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_padding, menu);
        return true;
    }

    private void toggleGps(boolean enable) {
        try {
            if (mMapboxMap != null) {
                // Enable user location
                mMapboxMap.setMyLocationEnabled(enable);

                TrackingSettings trackingSettings = mMapboxMap.getTrackingSettings();
                trackingSettings.setDismissTrackingOnGesture(false);
                trackingSettings.setMyLocationTrackingMode(enable ? MyLocationTracking.TRACKING_FOLLOW : MyLocationTracking.TRACKING_NONE);
            }
        } catch (SecurityException e) {
            // permission not granted is handled in MainActivity
            finish();
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                onBackPressed();
                return true;

            case R.id.action_user_tracking:
                toggleGps(true);
                return true;

            case R.id.action_bangalore:
                if (mMapboxMap != null) {
                    // Move to bangalore
                    toggleGps(false);
                    LatLng bangalore = new LatLng(12.9810816, 77.6368034);
                    mMapboxMap.moveCamera(CameraUpdateFactory.newCameraPosition(
                            new CameraPosition.Builder()
                                    .zoom(16)
                                    .target(bangalore)
                                    .bearing(40)
                                    .tilt(45)
                                    .build()));
                    mMapboxMap.addMarker(new MarkerOptions().title("Center map").position(bangalore));
                }
                return true;

            default:
                return super.onOptionsItemSelected(item);
        }
    }

}

