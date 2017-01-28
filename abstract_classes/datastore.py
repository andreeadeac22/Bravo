class Datastore:
    """Used for storing and querying data by location and time."""

    def get(location, time, radius=DEFAULT_RADIUS, type="IMAGE"):
        """Retrieves data for a certain location and time. Optional parameters include radius from location and type of data to retrieve."""
    
    def put(data, location, time, type="IMAGE"):
        """Puts data at a certain location and time. Optional parameter includes type of data to store."""
