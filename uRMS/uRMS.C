/**
 * \file
 * \author Dmitri Naumov (dmitri.naumov@ufz.de)
 *
 * \copyright 2012 -- Helmholtz Centre for Environmental Research - UFZ
 * distributed under the GNU GPL version 3. See the LICENSE file for details.
 *
 * \brief Root-mean-square of a vector field.
 *
 * Calculates the root-mean-sqare of a vector field, which name is given on
 * command line.
 */

#include "fvCFD.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    timeSelector::addOptions();

    #include "setRootCase.H"
    #include "createTime.H"

    instantList timeDirs = timeSelector::select0(runTime, args);

    const word vecFieldName = "U";

    #include "createMesh.H"

    runTime.setTime(timeDirs.last(), timeDirs.size()-1);


    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        Info<< "Time = " << runTime.timeName() << endl;

        Info << "    Read vector field " << vecFieldName << endl;

        IOobject vecFieldHeader
        (
            vecFieldName,
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );

        if (!vecFieldHeader.headerOk())
        {
            Info << "Unable to read vector field" << vecFieldName << endl;
            return EXIT_FAILURE;
        }

        const volVectorField vecField(vecFieldHeader, mesh);
        const volScalarField magVecField(magSqr(vecField));

        scalar volIntegral2 = 0;
        vector volIntegral(0, 0, 0);
        scalar volume = 0;

        forAll (magVecField, cellI)
        {
            volIntegral2 += magVecField[cellI]*magVecField.mesh().V()[cellI];
            volIntegral += vecField[cellI]*vecField.mesh().V()[cellI];
            volume += magVecField.mesh().V()[cellI];
        }
        Info << "Domain volume = " << volume << endl;
        Info << "Volume integral = " << volIntegral2 << endl;
        Info << "RMS(mag(" << vecFieldName << ")) = " << Foam::sqrt(volIntegral2/volume) << endl;
        Info << "avg(" << vecFieldName << ")) = " << volIntegral/volume;

        Info<< endl;
    }

    return 0;
}


// ************************************************************************* //
